#include "collector.h"

#include <fcntl.h>

#define COLLECTOR_SIZE_SUFFIXES   "BKMGTPEZY"
#define COLLECTOR_IO_BUFFER_SIZE    1024
#define COLLECTOR_IO_TEMP_SIZE      64
#define COLLECTOR_IO_PART_ID_READ_SECTORS   3
#define COLLECTOR_IO_PART_ID_WRITE_SECTORS  7
#define COLLECTOR_TEMP_BUFFER_SIZE  8
#define COLLECTOR_CPU_BUFFER_BASE   8192
#define COLLECTOR_CPU_BUFFER_MULTIPLY   1.5
#define COLLECTOR_CPU_MAX_NO    8
#define COLLECTOR_CPU_PROC_STAT "/proc/stat"

static const unsigned short collector_len_size_suffixes = strlen(COLLECTOR_SIZE_SUFFIXES);
static const char collector_size_suffixes[] = COLLECTOR_SIZE_SUFFIXES;

static const char collector_io_type_chars[3] = "rwm";


static const unsigned collector_parse_argument_minium_sep_count[] = {
    (unsigned)-1, // None
    2,  // String
    2,  // Temp
    3,  // IO
    1,  // CPU
    3,  // Net
};
size_t collector_size_to_human_readable(size_t size, char *suffix) {
    for (unsigned short i = 0; i < collector_len_size_suffixes; ++i) {
        if (size < 0x400) {
            *suffix = collector_size_suffixes[i];
            return size;
        }
        size /= 0x400;
    }
    *suffix = 'B';
    return size;
}

int collector_string_prepare(struct collector_string *const collector) {
    if (collector->loop) {
        collector->off = 0;
    }
    return 0;
}

int collector_string_report(struct collector_string *const collector, char report[5]) {
    if (collector->loop) {
        strncpy(report, collector->string + collector->off, 4);
        if (collector->off + 4 >= collector->len) {
            collector->off = 0;
        } else {
            ++collector->off;
        }
    } else {
        strncpy(report, collector->string, 4);
    }
    return 0;
}

int collector_temp_init(struct collector_temp *const collector) {
    char path[PATH_MAX];
    if (snprintf(path, PATH_MAX, "/sys/class/thermal/thermal_zone%hu/temp", collector->zone) < 0) {
        pr_error_with_errno("Failed to create sysfs stat path for thermal zone %hu", collector->zone);
        return 1;
    }
    if ((collector->stat_fd = open(path, O_RDONLY)) < 0) {
        pr_error_with_errno("Failed to open '%s' for read", path);
        return 2;
    }
    pr_warn("Initialized temperature collector for thermal zone %hu\n", collector->zone);
    return 0;
}

int collector_temp_report(struct collector_temp *const collector, char report[5]) {
    char buffer[COLLECTOR_TEMP_BUFFER_SIZE];
    if (lseek(collector->stat_fd, 0, SEEK_SET)) {
        pr_error_with_errno("Failed to seek to beginning of stat file for thermal zone %hu", collector->zone);
        return 1;
    }
    if (read(collector->stat_fd, buffer, COLLECTOR_TEMP_BUFFER_SIZE) < 0) {
        pr_error_with_errno("Failed to read temperature from thermal zone %hu", collector->zone);
        return 2;
    }
    unsigned long temp = strtoul(buffer, NULL, 10);
    if (temp >= 1000000) {
        strcpy(report, "xxxx");
    } else if (temp >= 100000) {
        snprintf(report, 5, "%3luC", temp / 1000);
    } else {
        snprintf(report, 5, "%2lu C", temp / 1000);
        report[2] = 0xb0;
    }
    return 0;
}

int collector_io_init(struct collector_io *const collector) {
    char path[PATH_MAX];
    if (snprintf(path, PATH_MAX, "/sys/block/%s/stat", collector->blkdev) < 0) {
        pr_error_with_errno("Failed to create sysfs stat path for block device '%s'", collector->blkdev);
        return 1;
    }
    if ((collector->stat_fd = open(path, O_RDONLY)) < 0) {
        pr_error_with_errno("Failed to open '%s' for read", path);
        pr_warn("attempting for sysfs subfolders\n");
        size_t len = strlen(collector->blkdev);
        if (len > NAME_MAX) {
            len = NAME_MAX;
        }
        char name[NAME_MAX + 1];
        strncpy(name, collector->blkdev, len + 1);
        while (len) {
            name[--len] = '\0';
            if (snprintf(path, PATH_MAX, "/sys/block/%s/%s/stat", name, collector->blkdev) < 0) {
                pr_error_with_errno("Failed to create sysfs stat path for block device '%s' potentially being child to block device '%s'", collector->blkdev, name);
                return 2;
            }
            if ((collector->stat_fd = open(path, O_RDONLY)) >= 0) {
                pr_warn("Opened '%s' for read for stat for block device '%s'\n", path, collector->blkdev);
                break;
            }
        }
        if (collector->stat_fd < 0) {
            pr_error_with_errno("All attempt to open sysfs stat file for block device '%s' failed", collector->blkdev);
            return 3;
        }
    }
    pr_warn("Initialized IO collector for block device '%s' type '%c'\n", collector->blkdev, collector_io_type_chars[collector->type]);
    return 0;
}

int collector_io_parse(struct collector_io *const collector) {
    uint8_t buffer[COLLECTOR_IO_BUFFER_SIZE];
    if (lseek(collector->stat_fd, 0, SEEK_SET) < 0) {
        pr_error_with_errno("Failed to seek to beginning of stat file for block device '%s'", collector->blkdev);
        return 1;
    }
    ssize_t len = read(collector->stat_fd, buffer, COLLECTOR_IO_BUFFER_SIZE);
    if (len < 0) {
        pr_error_with_errno("Failed to read from stat file for block device '%s'", collector->blkdev);
        return 2;
    }
    if (len == 0) {
        pr_error("Stat file for block device '%s' is empty\n", collector->blkdev);
        return 3;
    }
    // collector->read_sectors_this = 0;
    // collector->write_sectors_this = 0;
    uint8_t *part = NULL;
    uint8_t part_id = 0;
    for (uint8_t *byte = buffer;; ++byte) {
        switch (*byte) {
        case ' ':
        case '\n':
        case '\0':
            if (part) {
                switch (part_id) {
                case COLLECTOR_IO_PART_ID_READ_SECTORS:
                case COLLECTOR_IO_PART_ID_WRITE_SECTORS: {
                    // uint8_t temp[COLLECTOR_IO_TEMP_SIZE];
                    if ((len = byte - part) > COLLECTOR_IO_TEMP_SIZE) {
                        pr_error("Part of stat for block device '%s' is too long, %lu bytes exceeds temp size %d\n", collector->blkdev, len, COLLECTOR_IO_TEMP_SIZE);
                        return 4;
                    }
                    // strncpy((char *)temp, (char *)part, len);
                    // temp[len] = '\0';
                    size_t const sectors = strtoul((char *)part, NULL, 10);
                    if (part_id == COLLECTOR_IO_PART_ID_READ_SECTORS) {
                        collector->read_sectors_this = sectors;
                    } else {
                        collector->write_sectors_this = sectors;
                        return 0;
                    }
                }
                }
                part = NULL;
            }
            if (*byte != ' ') {
                pr_error("Unexpected EOL: %s\n", buffer);
                return 5;
            }
            break;
        default:
            if (!part) {
                part = byte;
                ++part_id;
            }
        }
    }
    return 5;
}

static inline int collector_io_prepare(struct collector_io *const collector) {
    if (collector_io_parse(collector)) {
        pr_error("Failed to prepare collector for IO for block device '%s'\n", collector->blkdev);
        return 1;
    }
    return 0;
}

static inline int collector_io_report(struct collector_io *const collector, char report[5]) {
    collector->read_sectors_last = collector->read_sectors_this;
    collector->write_sectors_last = collector->write_sectors_this;
    if (collector_io_parse(collector)) {
        pr_error("Failed to parse block device stat for '%s' for report\n", collector->blkdev);
        return 1;
    }
    size_t diff;
    switch (collector->type) {
        case COLLECTOR_IO_TYPE_READ:
            diff = collector->read_sectors_this - collector->read_sectors_last;
            break;
        case COLLECTOR_IO_TYPE_WRITE:
            diff = collector->write_sectors_this - collector->write_sectors_last;
            break;
        case COLLECTOR_IO_TYPE_MIXED:
            diff = collector->read_sectors_this + collector->write_sectors_this - collector->read_sectors_last - collector->write_sectors_last;
            break;
    }
    char suffix;
    diff = collector_size_to_human_readable(diff, &suffix);
    if (snprintf(report, 5, "%3lu%c", diff, suffix) < 0) {
        pr_error_with_errno("Failed to generate report string for human-readable size %lu%c\n", diff, suffix);
        return 3;
    }
    return 0;
}

int collector_cpu_safe_read(struct collector_cpu *const collector) {
    ssize_t size = 0;
    for (;;) {
        if (lseek(collector->stat_fd, 0, SEEK_SET) < 0) {
            pr_error_with_errno("Failed to seek in '"COLLECTOR_CPU_PROC_STAT"'");
            return 1;
        }
        if ((size = read(collector->stat_fd, collector->buffer, collector->alloc)) < 0) {
            pr_error_with_errno("Failed to read %lu bytes from '"COLLECTOR_CPU_PROC_STAT"'", collector->alloc);
            return 2;
        }
        if (collector->alloc > (size_t)size) {
            return 0;
        }
        char *buffer = realloc(collector->buffer, collector->alloc *= COLLECTOR_CPU_BUFFER_MULTIPLY);
        if (buffer) {
            collector->buffer = buffer;
        } else {
            pr_error_with_errno("Failed to re-allocate memory for CPU stat");
            return 3;
        }
    }
}

int collector_cpu_init(struct collector_cpu *const collector) {
    if ((collector->stat_fd = open(COLLECTOR_CPU_PROC_STAT, O_RDONLY)) < 0) {
        pr_error_with_errno("Failed to open '"COLLECTOR_CPU_PROC_STAT"' for read to prepare for CPU usage info");
        return 1;
    }
    if (!(collector->buffer = malloc(COLLECTOR_CPU_BUFFER_BASE))) {
        pr_error_with_errno("Failed to allocate memory for CPU stat");
        close(collector->stat_fd);
        return 2;
    }
    collector->alloc = COLLECTOR_CPU_BUFFER_BASE;
    if (collector_cpu_safe_read(collector)) {
        pr_error("Failed to read into memory\n");
        free(collector->buffer);
        close(collector->stat_fd);
        return 3;
    }
    bool head = true;
    char *line = collector->buffer;
    for (char *c = collector->buffer;; ++c) {
        switch (*c) {
        case ' ':
            if (head) {
                if (!strncmp(line, collector->label, c - line + 1)) { // Plus one to also contain space
                    return 0;
                }
                head = false;
            }
            break;
        case '\0':
            pr_error("Failed to find line for cpu stat for '%s'\n", collector->label);
            return 4;
        case '\n':
            head = true;
            line = c + 1;
            break;
        default:
            break;
        }
    }
}

int collector_cpu_parse(struct collector_cpu *const collector) {
    if (collector_cpu_safe_read(collector)) {
        return 1;
    }
    collector->idle_this = 0;
    collector->busy_this = 0;
    bool parse_line = true;
    char *part = NULL;
    unsigned part_id = 0;
    for (char *c = collector->buffer;; ++c) {
        switch (*c) {
        case '\0':
            pr_error("Failed to find line for cpu stat for '%s'\n", collector->label);
            return 1;
        case ' ':
        case '\n':
            if (parse_line) {
                switch (part_id) {
                    case 1: /* cpu */
                        if (strncmp(part, collector->label, c - part + 1)) {
                            parse_line = false;
                        }
                        break;
                    case 2: /* user */
                    case 3: /* nice */
                    case 4: /* system */
                    case 10: /* guest */
                    case 11: /* guest_nice */
                        collector->busy_this += strtoul(part, NULL, 10);
                        break;
                    case 5: /* idle */
                    case 6: /* iowait */
                    case 7: /* irq */
                    case 8: /* softirq */
                    case 9: /* steal */
                        collector->idle_this += strtoul(part, NULL, 10);
                        break;
                    default:
                        pr_error("Unexpected line part id: %u\n", part_id);
                        return 2;
                }
                if (*c == '\n' || part_id == 11) {
                    return 0;
                }
            }
            part = NULL;
            if (*c == '\n') {
                parse_line = true;
                part_id = 0;
            }
            break;
        default:
            if (parse_line && !part) {
                part = c;
                ++part_id;
            }
            break;
        }
    }
    return 0;
}

int collector_cpu_prepare(struct collector_cpu *const collector) {
    if (collector_cpu_parse(collector)) {
        pr_error("Failed to parse cpu load info\n");
        return 1;
    }
    return 0;
}

int collector_cpu_report(struct collector_cpu *const collector, char report[5]) {
    collector->idle_last = collector->idle_this;
    collector->busy_last = collector->busy_last;
    if (collector_cpu_parse(collector)) {
        pr_error("Failed to parse cpu load info\n");
        return 1;
    }
    unsigned long const busy_diff = collector->busy_this - collector->busy_last;
    unsigned long const total_diff = busy_diff + collector->idle_this - collector->idle_last;
    unsigned short const percent = busy_diff / total_diff * 100;
    if (percent >= 100) {
        strcpy(report, "100%");
    } else {
        snprintf(report, 5, "%2hu o", percent);
        report[2] = 0xb0;
    }
    return 0;
}

int collector_init(struct collector const collector) {
    switch (collector.type) {
    case COLLECTOR_TYPE_STRING:
        return 0;
    case COLLECTOR_TYPE_TEMP:
        return collector_temp_init(collector.temp);
    case COLLECTOR_TYPE_IO:
        return collector_io_init(collector.io);
    case COLLECTOR_TYPE_CPU:
        return collector_cpu_init(collector.cpu);
    case COLLECTOR_TYPE_NET:
        return 0;
    default:
        pr_error("Unexpected collector type %d\n", collector.type);
        return -1;
    }
}

int collector_prepare(struct collector const collector) {
    switch (collector.type) {
    case COLLECTOR_TYPE_STRING:
        return collector_string_prepare(collector.string);
    case COLLECTOR_TYPE_TEMP:
        return 0;
    case COLLECTOR_TYPE_IO:
        return collector_io_prepare(collector.io);
    case COLLECTOR_TYPE_CPU:
        return collector_cpu_prepare(collector.cpu);
    case COLLECTOR_TYPE_NET:
        return 0;
    default:
        pr_error("Unexpected collector type %d\n", collector.type);
        return -1;
    }
}

int collector_report(struct collector const collector, char report[5]) {
    switch (collector.type) {
    case COLLECTOR_TYPE_STRING:
        return collector_string_report(collector.string, report);
    case COLLECTOR_TYPE_TEMP:
        return collector_temp_report(collector.temp, report);
    case COLLECTOR_TYPE_IO:
        return collector_io_report(collector.io, report);
    case COLLECTOR_TYPE_CPU:
        return collector_cpu_report(collector.cpu, report);
    case COLLECTOR_TYPE_NET:
        return 0;
    default:
        pr_error("Unexpected collector type %d\n", collector.type);
        return -1;
    }
}


struct collector_string *collector_parse_argument_string(char const *const start, char const *const end) {
    if (start >= end) {
        pr_error("String collector has empty string\n");
        return NULL;
    }
    size_t const len = end - start;
    struct collector_string *collector;
    if (len >= sizeof collector->string) {
        pr_error("String collector too long (%lu but at most %lu): '%s'\n", len, sizeof collector->string - 1, start);
        return NULL;
    }
    if (!(collector = malloc(sizeof *collector))) {
        pr_error_with_errno("Failed to allocate memory for string collector");
        return NULL;
    }
    strncpy(collector->string, start, sizeof collector->string);
    collector->len = len;
    collector->loop = len > 4;
    return collector;
}

struct collector_temp *collector_parse_argument_temp(char const *const start, char const *const end) {
    if (start >= end) {
        pr_error("Thermal zone is not defined\n");
        return NULL;
    }
    struct collector_temp *collector = malloc(sizeof *collector);
    if (!collector) {
        pr_error_with_errno("Failed to allocate memory for temperature collector");
        return NULL;
    }
    collector->zone = strtoul(start, NULL, 10);
    return collector;
}

struct collector_io *collector_parse_argument_io(char const *const sep_type_blkdev, char const *const sep_blkdev_rw, char const *const end) {
    if (sep_blkdev_rw <= sep_type_blkdev + 1) {
        pr_error("Block device is not defined\n");
        return NULL;
    }
    if (end <= sep_blkdev_rw + 1) {
        pr_error("R/W type is not defined");
        return NULL;
    }
    enum collector_io_type type;
    switch (*(sep_blkdev_rw + 1)) {
    case 'r':
    case 'R':
        type = COLLECTOR_IO_TYPE_READ;
        break;
    case 'w':
    case 'W':
        type = COLLECTOR_IO_TYPE_WRITE;
        break;
    case 'm':
    case 'M':
        type = COLLECTOR_IO_TYPE_MIXED;
        break;
    default:
        pr_error("R/W type illegal: %c, should be either one of r, R, w, W, m, M\n", *(sep_blkdev_rw + 1));
        return NULL;
    }
    size_t len_blkdev = sep_blkdev_rw - sep_type_blkdev - 1;
    struct collector_io *collector;
    if (len_blkdev >= sizeof collector->blkdev) {
        pr_error("Block device name too long, length %lu but at most %lu: '%s'\n", len_blkdev, sizeof collector->blkdev, sep_type_blkdev + 1);
        return NULL;
    }
    if (!(collector = malloc(sizeof *collector))) {
        pr_error_with_errno("Failed to allocate memory for I/O collector");
        return NULL;
    }
    strncpy(collector->blkdev, sep_type_blkdev + 1, len_blkdev);
    collector->blkdev[len_blkdev] = '\0';
    collector->type = type;
    return collector;
}

struct collector_cpu *collector_parse_argument_cpu(char const *const sep_type_cpuno) {
    int cpu_no = -1;
    if (*sep_type_cpuno) {
        switch (*(sep_type_cpuno + 1)) {
        case '\0':
        case '-':
            break;
        default:
            cpu_no = strtoul(sep_type_cpuno + 1, NULL, 10);
        }
    }
    struct collector_cpu *collector = malloc(sizeof *collector);
    if (!collector) {
        pr_error_with_errno("Failed to allocate memory for CPU collector");
        return NULL;
    }
    if (cpu_no >= 0 && cpu_no < COLLECTOR_CPU_MAX_NO) {
        collector->cpu_no = cpu_no;
        if (snprintf(collector->label, sizeof collector->label, "cpu%d ", collector->cpu_no) < 0) {
            pr_error_with_errno("Failed to generate label for cpu no.%d", collector->cpu_no);
            free(collector);
            return NULL;
        }
    } else {
        collector->cpu_no = -1;
        strcpy(collector->label, "cpu ");
    }
    return collector;
}

struct collector_net *collector_parse_argument_net(char const *const sep_type_iface, char const *const sep_iface_tr, char const *const end) {
    if (sep_iface_tr <= sep_type_iface + 1) {
        pr_error("Interface device is not defined\n");
        return NULL;
    }
    if (end <= sep_iface_tr + 1) {
        pr_error("Tx/Rx type is not defined");
        return NULL;
    }
    enum collector_net_type type;
    switch (*(sep_iface_tr + 1)) {
    case 't':
    case 'T':
        type = COLLECTOR_NET_TYPE_TX;
        break;
    case 'r':
    case 'R':
        type = COLLECTOR_NET_TYPE_RX;
        break;
    case 'm':
    case 'M':
        type = COLLECTOR_NET_TYPE_MIXED;
        break;
    default:
        pr_error("Tx/Rx type illegal: %c, should be either one of t, T, r, R, m, M\n", *(sep_iface_tr + 1));
        return NULL;
    }
    size_t len_iface = sep_iface_tr - sep_type_iface - 1;
    struct collector_net *collector;
    if (len_iface >= sizeof collector->interface) {
        pr_error("Network interface  name too long, length %lu but at most %lu: '%s'\n", len_iface, sizeof collector->interface, sep_type_iface + 1);
        return NULL;
    }
    if (!(collector = malloc(sizeof *collector))) {
        pr_error_with_errno("Failed to allocate memory for network collector");
        return NULL;
    }
    strncpy(collector->interface, sep_type_iface + 1, len_iface);
    collector->interface[len_iface] = '\0';
    collector->type = type;
    return collector;
}


int collector_parse_argument(struct collector *collector, char const *const arg, char const *const seps[], unsigned const sep_count, char const *const end) {
    if (!collector) {
        pr_error("Collector not allocated yet\n");
        return -1;
    }
    if (sep_count < collector_parse_argument_minium_sep_count[collector->type]) {
        pr_error("Argument too short, expecting at least %u seperators but only %u: '%s'\n", collector_parse_argument_minium_sep_count[collector->type], sep_count, arg);
        return 1;
    }
    switch (collector->type) {
    case COLLECTOR_TYPE_STRING:
        collector->string = collector_parse_argument_string(seps[1] + 1, end);
        break;
    case COLLECTOR_TYPE_TEMP:
        collector->temp = collector_parse_argument_temp(seps[1] + 1, end);
        break;
    case COLLECTOR_TYPE_IO:
        collector->io = collector_parse_argument_io(seps[1], seps[2], end);
        break;
    case COLLECTOR_TYPE_CPU:
        collector->cpu = collector_parse_argument_cpu(seps[1]);
        break;
    case COLLECTOR_TYPE_NET:
        collector->net = collector_parse_argument_net(seps[1], seps[2], end);
        break;
    default:
        pr_error("Collector type not set: '%s'\n", arg);
        return 1;
    }
    if (!collector->raw) {
        pr_error("Failed to parse argument to collector: '%s'\n", arg);
        return 2;
    }
    return 0;
}
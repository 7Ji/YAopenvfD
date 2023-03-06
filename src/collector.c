#include "collector.h"

#include <fcntl.h>

// unsigned collector_get_count(struct collector_raw const *const collector_head) {
//     unsigned count = 0;
//     for (struct collector_raw const *collector = collector_head; collector; collector = collector->next) {
//         ++count;
//     }
//     return count;
// }
// enum collector_type collector_type_from_reporter_type(enum reporter_type const type) {
//     return (enum collector_type) type;
// }

#define COLLECTOR_SIZE_SUFFIXES   "BKMGTPEZY"
static const unsigned short collector_len_size_suffixes = strlen(COLLECTOR_SIZE_SUFFIXES);
static const char collector_size_suffixes[] = COLLECTOR_SIZE_SUFFIXES;

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

// int collector_string(struct collector_string *const collector_head) {
//     // unsigned count = collector_get_count((struct collector_raw const *const)collector_head);
//     // if (!count) {
//     //     pr_error("No collector defined\n");
//     //     return -1;
//     // }
//     return 0;
// }

// int collector_temp(struct collector_temp *const collector_head) {
//     // unsigned count = collector_get_count((struct collector_raw const *const)collector_head);
//     // if (!count) {
//     //     pr_error("No collector defined\n");
//     //     return -1;
//     // }
//     return 0;
// }

int collector_io_init(struct collector_io *const collector) {
    char path[PATH_MAX];
    if (snprintf(path, PATH_MAX, "/sys/block/%s/stat", collector->blkdev) < 0) {
        pr_error_with_errno("Failed to create sysfs stat path for block device '%s'", collector->blkdev);
        return 1;
    }
    if ((collector->stat_fd = open(path, O_RDONLY)) < 0) {
        pr_error_with_errno("Failed open '%s' for read, attempting for sysfs subfolders", path);
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
    return 0;
}

#define COLLECTOR_IO_BUFFER_SIZE    1024
#define COLLECTOR_IO_TEMP_SIZE   64
#define COLLECTOR_IO_PART_ID_READ_SECTORS   3
#define COLLECTOR_IO_PART_ID_WRITE_SECTORS  7

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
                    uint8_t temp[COLLECTOR_IO_TEMP_SIZE];
                    if ((len = byte - part) > COLLECTOR_IO_TEMP_SIZE) {
                        pr_error("Part of stat for block device '%s' is too long, %lu bytes exceeds temp size %d\n", collector->blkdev, len, COLLECTOR_IO_TEMP_SIZE);
                        return 4;
                    }
                    strncpy((char *)temp, (char *)part, len);
                    temp[len] = '\0';
                    size_t const sectors = strtoul((char *)temp, NULL, 10);
                    if (part_id == COLLECTOR_IO_PART_ID_READ_SECTORS) {
                        collector->read_sectors_last = collector->read_sectors_this;
                        collector->read_sectors_this = sectors;
                    } else {
                        collector->write_sectors_last = collector->write_sectors_this;
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
    collector->read_sectors_this = 0;
    collector->write_sectors_this = 0;
    if (collector_io_parse(collector)) {
        pr_error("Failed to prepare collector for IO for block device '%s'\n", collector->blkdev);
        return 1;
    }
    return 0;
}

static inline int collector_io_report(struct collector_io *const collector, char report[5]) {
    int r = collector_io_parse(collector);
    if (r) {
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
        /*default:
            pr_error("Collector IO type un-recognized: %c (0x%x), expecting one of r, w, m\n", collector->type, collector->type);
            return 2;*/
    }
    char suffix;
    diff = collector_size_to_human_readable(diff, &suffix);
    if (snprintf(report, 5, "%lu%c", diff, suffix) < 0) {
        pr_error_with_errno("Failed to generate report string for human-readable size %lu%c\n", diff, suffix);
        return 3;
    }
    return 0;
}

// int collector_io_prepare() {
//     int r = collector_io_parse();
//     if (r) {
//         return 1;
//     }
// }

// int collector_io_report(char *report) {
//     int r = collector_io_parse();

// }

int collector_prepare(struct collector const collector) {
    switch (collector.type) {
    case COLLECTOR_TYPE_STRING:
    case COLLECTOR_TYPE_TEMP:
        return 0;
    case COLLECTOR_TYPE_IO:
        return collector_io_prepare(collector.io);
    case COLLECTOR_TYPE_CPU:
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
    case COLLECTOR_TYPE_TEMP:
        return 0;
    case COLLECTOR_TYPE_IO:
        return collector_io_report(collector.io, report);
    case COLLECTOR_TYPE_CPU:
    case COLLECTOR_TYPE_NET:
        return 0;
    default:
        pr_error("Unexpected collector type %d\n", collector.type);
        return -1;
    }
}

/*
enum collector_io_parse_size_type {
    COLLECTOR_IO_PARSE_SIZE_TYPE_READ,
    COLLECTOR_IO_PARSE_SIZE_TYPE_WRITE,
    COLLECTOR_IO_PARSE_SIZE_TYPE_MIXED
};

#define COLLECTOR_IO_PARSE_SIZE_BUFFER_SIZE 128

static inline void collector_io_parse_size(struct collector_io *collector, bool is_read, char *start, size_t len) {
    char buffer[COLLECTOR_IO_PARSE_SIZE_BUFFER_SIZE];
    if (len > COLLECTOR_IO_PARSE_SIZE_BUFFER_SIZE - 1) {
        len = COLLECTOR_IO_PARSE_SIZE_BUFFER_SIZE - 1;
    }
    strncpy(buffer, start, len);
    buffer[len] = '\0';
    size_t const sectors = strtoul(buffer, NULL, 10);
    size_t const size_diff = (sectors - (is_read ? collector->read_sectors : collector->write_sectors)) * 512;
#ifdef DEBUGGING
    char suffix;
    size_t size_human_readable = util_size_to_human_readable(size_diff, &suffix);
#endif
    pr_debug("%s speed: %lu%c/s\n", is_read ? "Read" : "Write", size_human_readable, suffix);
    if (is_read) {
        collector->read_sectors = sectors;
    } else {
        collector->write_sectors = sectors;
    }
}

static inline void collector_io_parse(struct collector_io *collector_head, char *buffer) {
    unsigned short part_id = 0;
    char *part = NULL;
    bool parse_line = false;
    struct collector_io *collector;
    for (char *c = buffer;; ++c) {
        switch (*c) {
            case ' ':
                if (part) {
                    bool parse_part;
                    switch(part_id) {
                        case COLLECTOR_IO_ID_NAME:
                        case COLLECTOR_IO_ID_READ:
                        case COLLECTOR_IO_ID_WRITE:
                            parse_part = true;
                            break;
                        default:
                            parse_part = false;
                            break;
                    }
                    if (parse_part) {
                        size_t len = c - part;
                        switch (part_id) {
                            case COLLECTOR_IO_ID_NAME: {
                                for (collector = collector_head; collector; collector = collector->next) {
                                    if (strncmp(collector->blkdev, part, len) || collector->blkdev[len]) {
                                        continue;
                                    }
                                    pr_debug("Start parsing I/O for block device %s\n", collector->blkdev);
                                    parse_line = true;
                                    break;
                                }
                                break;
                            }
                            case COLLECTOR_IO_ID_READ:
                                if (parse_line) {
                                    collector_io_parse_size(collector, true, part, len);
                                }
                                break;
                            case COLLECTOR_IO_ID_WRITE:
                                if (parse_line) {
                                    collector_io_parse_size(collector, false, part, len);
                                }
                                break;
                            default:
                                break;
                        }
                    }
                    part = NULL;
                }
                break;
            case '\0':
                return;
            case '\n':
                part = NULL;
                part_id = 0;
                parse_line = false;
                break;
            default:
                if (!part) {
                    part = c;
                    ++part_id;
                }
        }

    }
}

int collector_io(struct collector_io *collector_head) {
    unsigned count = collector_get_count((struct collector const *const)collector_head);
    if (!count) {
        pr_error("No collector defined\n");
        return -1;
    }
    int fd _cleanup_(close_p)= open(COLLECTOR_IO_DISKSTAT, O_RDONLY);
    if (fd < 0) {
        pr_error_with_errno("Failed to open '"COLLECTOR_IO_DISKSTAT"' to read disk I/O");
        return 1;
    }
    size_t alloc = COLLECTOR_IO_ALLOC_BASE;
    char *buffer _cleanup_(free_p) = malloc(alloc);
    if (!buffer) {
        pr_error_with_errno("Failed to allocate memory for I/O buffer");
        return 2;
    }
    while (true) {
        ssize_t len = read(fd, buffer, alloc);
        if (len < 0) {
            pr_error_with_errno("Failed to read from '"COLLECTOR_IO_DISKSTAT"'");
            return 3;
        }
        if ((size_t)len == alloc) {
            alloc *= COLLECTOR_IO_ALLOC_MULTIPLY;
            char *buffer_new = realloc(buffer, alloc);
            if (buffer_new) {
                buffer = buffer_new;
                if (lseek(fd, 0, SEEK_SET) == -1) {
                    pr_error_with_errno("Failed to seek back to beginning of '"COLLECTOR_IO_DISKSTAT"'");
                    return 4;
                }
                continue;
            } else {
                pr_error_with_errno("Failed to allocate more memory for I/O from '"COLLECTOR_IO_DISKSTAT"'");
                return 5;
            }
        }
        collector_io_parse(collector_head, buffer);
        sleep(1);
        if (lseek(fd, 0, SEEK_SET) == -1) {
            pr_error_with_errno("Failed to seek back to beginning of '"COLLECTOR_IO_DISKSTAT"'");
            return 6;
        }
    }
    return 0;
}
*/

// int collector_cpu(struct collector_cpu *collector_head) {
//     // unsigned count = collector_get_count((struct collector const *const)collector_head);
//     // if (!count) {
//     //     pr_error("No collector defined\n");
//     //     return -1;
//     // }
//     return 0;
// }

// int collector_net(struct collector_net *collector_head) {
//     // unsigned count = collector_get_count((struct collector const *const)collector_head);
//     // if (!count) {
//     //     pr_error("No collector defined\n");
//     //     return -1;
//     // }
//     return 0;
// }

// int collector_all() {
//     return 0;
// }

static const unsigned collector_parse_argument_minium_sep_count[] = {
    (unsigned)-1, // None
    2,  // String
    2,  // Temp
    3,  // IO
    2,  // CPU
    3,  // Net
};

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
        break;
    case COLLECTOR_TYPE_NET:
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
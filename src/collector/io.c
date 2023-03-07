#include "collector/io.h"
#include "collector/size.h"
#include <fcntl.h>

#define COLLECTOR_IO_BUFFER_SIZE    1024
#define COLLECTOR_IO_TEMP_SIZE      64
#define COLLECTOR_IO_PART_ID_READ_SECTORS   3
#define COLLECTOR_IO_PART_ID_WRITE_SECTORS  7

#define COLLECTOR_IO_DISKSTAT "/proc/diskstats"
#define COLLECTOR_IO_ALLOC_BASE 0x1000
#define COLLECTOR_IO_ALLOC_MULTIPLY 2

#define COLLECTOR_DEFINER_IO_BLKDEV_MAXLEN  64

#define COLLECTOR_IO_ID_NAME    3
#define COLLECTOR_IO_ID_READ    6
#define COLLECTOR_IO_ID_WRITE   10

static const char collector_io_type_chars[3] = "rwm";

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

int collector_io_prepare(struct collector_io *const collector) {
    if (collector_io_parse(collector)) {
        pr_error("Failed to prepare collector for IO for block device '%s'\n", collector->blkdev);
        return 1;
    }
    return 0;
}

int collector_io_report(struct collector_io *const collector, char report[COLLECTOR_REPORT_SIZE]) {
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
    diff = collector_size_to_human_readable(diff * 512, &suffix);
    if (snprintf(report, 5, "%3lu%c", diff, suffix) < 0) {
        pr_error_with_errno("Failed to generate report string for human-readable size %lu%c\n", diff, suffix);
        return 3;
    }
    return 0;
}

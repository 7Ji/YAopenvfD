#include "collector.h"

#include <fcntl.h>

#include "util.h"

char const collector_type_strings[][7] = {
    "string",
    "temp",
    "io",
    "cpu",
    "net"
};


static inline bool collector_is_type_right(struct collector *collector, enum collector_type type) {
    if (collector->type == type) {
        return true;
    } else {
        pr_error("collector type not right, expected %u (%s) but got %u (%s)\n", type, collector_type_strings[type], collector->type, collector_type_strings[collector->type]);
        return false;
    }
}

int collector_string(struct collector *collector) {
    if (!collector_is_type_right(collector, COLLECTOR_TYPE_STRING)) {
        return -1;
    }
    // struct collector_definer_string *const definer = collector->definer.string;

    return 0;
}

int collector_temp(struct collector *collector) {
    if (!collector_is_type_right(collector, COLLECTOR_TYPE_TEMP)) {
        return -1;
    }
    // struct collector_definer_temp *const definer = collector->definer.temp;

    return 0;
}


enum collector_io_parse_size_type {
    COLLECTOR_IO_PARSE_SIZE_TYPE_READ,
    COLLECTOR_IO_PARSE_SIZE_TYPE_WRITE,
    COLLECTOR_IO_PARSE_SIZE_TYPE_MIXED
};

#define COLLECTOR_IO_PARSE_SIZE_BUFFER_SIZE 128

static inline void collector_io_parse_size(struct collector *collector, bool is_read, char *start, size_t len) {
    char buffer[COLLECTOR_IO_PARSE_SIZE_BUFFER_SIZE];
    if (len > COLLECTOR_IO_PARSE_SIZE_BUFFER_SIZE - 1) {
        len = COLLECTOR_IO_PARSE_SIZE_BUFFER_SIZE - 1;
    }
    strncpy(buffer, start, len);
    buffer[len] = '\0';
    size_t size = strtoul(buffer, NULL, 10);
    size_t size_diff;
    size_t size_human_readable;
    char suffix;
    if (is_read) {
        size_diff = (size - collector->storage.io->read) * 512;
        size_human_readable = util_size_to_human_readable(size_diff, &suffix);
        pr_debug("Read speed: %lu%c/s\n", size_human_readable, suffix);
        collector->storage.io->read = size;
    } else {
        size_diff = (size - collector->storage.io->write) * 512;
        size_human_readable = util_size_to_human_readable(size_diff, &suffix);
        pr_debug("Write speed: %lu%c/s\n", size_human_readable, suffix);
        collector->storage.io->write = size;
    }
}

static inline void collector_io_parse(struct collector *collector, char *buffer) {
    unsigned short part_id = 0;
    char *part = NULL;
    bool parse_line = false;
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
                                for (struct collector_definer_io *definer = collector->definer.io; definer; definer = definer->next) {
                                    if (strncmp(definer->blkdev, part, len) || definer->blkdev[len]) {
                                        continue;
                                    }
                                    pr_debug("Start parsing I/O for block device %s\n", definer->blkdev);
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

int collector_io(struct collector *collector) {
    if (!collector_is_type_right(collector, COLLECTOR_TYPE_IO)) {
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
        collector_io_parse(collector, buffer);
        sleep(1);
        if (lseek(fd, 0, SEEK_SET) == -1) {
            pr_error_with_errno("Failed to seek back to beginning of '"COLLECTOR_IO_DISKSTAT"'");
            return 6;
        }
    }
    return 0;
}

int collector_cpu(struct collector *collector) {
    if (!collector_is_type_right(collector, COLLECTOR_TYPE_CPU)) {
        return -1;
    }
    // struct collector_definer_cpu *const definer = collector->definer.cpu;

    return 0;
}

int collector_net(struct collector *collector) {
    if (!collector_is_type_right(collector, COLLECTOR_TYPE_NET)) {
        return -1;
    }
    // struct collector_definer_net *const definer = collector->definer.net;
    return 0;
}

int collector_all() {
    return 0;
}
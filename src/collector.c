#include "collector.h"

#include <fcntl.h>

#include "util.h"

unsigned collector_get_count(struct collector const *const collector_head) {
    unsigned count = 0;
    for (struct collector const *collector = collector_head; collector; collector = collector->next) {
        ++count;
    }
    return count;
}

int collector_string(struct collector_string *const collector_head) {
    unsigned count = collector_get_count((struct collector const *const)collector_head);
    if (!count) {
        pr_error("No collector defined\n");
        return -1;
    }
    return 0;
}

int collector_temp(struct collector_temp *const collector_head) {
    unsigned count = collector_get_count((struct collector const *const)collector_head);
    if (!count) {
        pr_error("No collector defined\n");
        return -1;
    }
    return 0;
}


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

int collector_cpu(struct collector_cpu *collector_head) {
    unsigned count = collector_get_count((struct collector const *const)collector_head);
    if (!count) {
        pr_error("No collector defined\n");
        return -1;
    }
    return 0;
}

int collector_net(struct collector_net *collector_head) {
    unsigned count = collector_get_count((struct collector const *const)collector_head);
    if (!count) {
        pr_error("No collector defined\n");
        return -1;
    }
    return 0;
}

int collector_all() {
    return 0;
}
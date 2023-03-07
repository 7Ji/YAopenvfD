/*
YAopenvfD/src/collector/cpu.c: logic to collect CPU usage
Copyright (C) 2023 Guoxin "7Ji" Pu <pugokushin@gmail.com>

This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with this program. If not, see <https://www.gnu.org/licenses/>.
*/

#include "collector/cpu.h"
#include <fcntl.h>

#define COLLECTOR_CPU_BUFFER_BASE   8192
#define COLLECTOR_CPU_BUFFER_MULTIPLY   1.5
#define COLLECTOR_CPU_MAX_NO    8
#define COLLECTOR_CPU_PROC_STAT "/proc/stat"

struct collector_cpu *collector_parse_argument_cpu(char const *const sep_type_cpuno) {
    int cpu_no = -1;
    if (sep_type_cpuno && *sep_type_cpuno) {
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
            if (parse_line && part) {
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
}

int collector_cpu_prepare(struct collector_cpu *const collector) {
    if (collector_cpu_parse(collector)) {
        pr_error("Failed to parse cpu load info\n");
        return 1;
    }
    return 0;
}

int collector_cpu_report(struct collector_cpu *const collector, char report[COLLECTOR_REPORT_SIZE]) {
    collector->idle_last = collector->idle_this;
    collector->busy_last = collector->busy_this;
    if (collector_cpu_parse(collector)) {
        pr_error("Failed to parse cpu load info\n");
        return 1;
    }
    unsigned long const busy_diff = collector->busy_this - collector->busy_last;
    unsigned long const total_diff = busy_diff + collector->idle_this - collector->idle_last;
    unsigned short const percent = busy_diff * 100 / total_diff ;
    if (percent >= 100) {
        strcpy(report, "100%");
    } else {
        snprintf(report, COLLECTOR_REPORT_SIZE, "%2hu o", percent);
        report[2] = 0xb0;
    }
    return 0;
}

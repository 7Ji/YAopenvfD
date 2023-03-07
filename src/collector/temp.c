/*
YAopenvfD/src/collector/temp.c: logic to collect temperature
Copyright (C) 2023 Guoxin "7Ji" Pu <pugokushin@gmail.com>

This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with this program. If not, see <https://www.gnu.org/licenses/>.
*/

#include "collector/temp.h"
#include <fcntl.h>

#define COLLECTOR_TEMP_BUFFER_SIZE  8

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

int collector_temp_report(struct collector_temp *const collector, char report[COLLECTOR_REPORT_SIZE]) {
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
        snprintf(report, COLLECTOR_REPORT_SIZE, "%3luC", temp / 1000);
    } else {
        snprintf(report, COLLECTOR_REPORT_SIZE, "%2lu C", temp / 1000);
        report[2] = 0xb0;
    }
    return 0;
}
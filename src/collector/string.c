/*
YAopenvfD/src/collector/string.c: logic to collect string to report
Copyright (C) 2023 Guoxin "7Ji" Pu <pugokushin@gmail.com>

This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with this program. If not, see <https://www.gnu.org/licenses/>.
*/

#include "collector/string.h"

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

int collector_string_prepare(struct collector_string *const collector) {
    if (collector->loop) {
        collector->off = 0;
    }
    return 0;
}

int collector_string_report(struct collector_string *const collector, char report[COLLECTOR_REPORT_SIZE]) {
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
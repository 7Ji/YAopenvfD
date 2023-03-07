/*
YAopenvfD/src/reporter.c: logic to report time/date/io/string/etc
Copyright (C) 2023 Guoxin "7Ji" Pu <pugokushin@gmail.com>

This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with this program. If not, see <https://www.gnu.org/licenses/>.
*/

#include "reporter.h"
#include "openvfd.h"

#define REPORTER_TYPE_MAX REPORTER_TYPE_DATE
#define REPORTER_PARSE_ARGUMENT_MAX_LEN 128
#define REPORTER_PARSE_ARGUMENT_MAX_SEP 3

static const char reporter_type_strings[][7] = {
    "",
    "string",
    "temp",
    "io",
    "cpu",
    "net",
    "date"
};

char const *reporter_get_type_string(enum reporter_type const type) {
    return reporter_type_strings[type];
}

enum reporter_type reporter_get_type_from_string(char const *const string) {
    if (!string || !string[0]) {
        return REPORTER_TYPE_NONE;
    }
    for (enum reporter_type type = REPORTER_TYPE_NONE + 1; type <= REPORTER_TYPE_MAX; ++type) {
        if (!strcmp(reporter_type_strings[type], string)) {
            return type;
        }
    }
    return REPORTER_TYPE_NONE;
}

static inline size_t reporter_parse_argument_safe_len(size_t len) {
    if (len > REPORTER_PARSE_ARGUMENT_MAX_LEN - 1) {
        return REPORTER_PARSE_ARGUMENT_MAX_LEN - 1;
    } else {
        return len;
    }
}

struct reporter *reporter_parse_argument(char const *const arg) {
    if (!arg || !arg[0]) {
        pr_error("Argument empty or not defined\n");
        return NULL;
    }
    char const *seps[REPORTER_PARSE_ARGUMENT_MAX_SEP] = {NULL};
    char const *dots = NULL;
    char const *end = NULL;
    unsigned sep_id = 0;
    for (char const *c = arg; !end; ++c) {
        switch (*c) {
        case ':':
            if (sep_id < REPORTER_PARSE_ARGUMENT_MAX_SEP) {
                seps[sep_id++] = c;
            }
            break;
        case '@':
            if (!sep_id) {
                dots = c + 1;
            }
            break;
        case '\0':
            end = c;
            break;
        default:
            break;
        }
    }
    if (sep_id < 1) {
        pr_error("Argument too short, expecting at least 1 seperator: '%s'\n", arg);
        return NULL;
    }
    if (!end) {
        pr_error("Argument not correctly ended: '%s'\n", arg);
        return NULL;
    }
    char temp[REPORTER_PARSE_ARGUMENT_MAX_LEN];
    size_t len = reporter_parse_argument_safe_len((sep_id > 1 ? seps[1] : end) - seps[0] - 1);
    strncpy(temp, seps[0] + 1, len);
    temp[len] = '\0';
    enum reporter_type const reporter_type = reporter_get_type_from_string(temp);
    if (!reporter_type) {
        pr_error("Argument does not define valid reporter type: '%s'\n", arg);
        return NULL;
    }
    enum dots_type dots_type;
    if (dots) {
        len = reporter_parse_argument_safe_len(seps[0] - dots);
        strncpy(temp, dots, len);
        temp[len] = '\0';
        if (!(dots_type = dots_get_type_from_string(temp))) {
            pr_error("Argument does not define valid dots type: '%s'\n", arg);
            return NULL;
        }
    } else {
        dots_type = DOTS_TYPE_NONE;
    }
    struct collector collector = {0};
    collector.type = (enum collector_type) reporter_type;
    if (collector_parse_argument(&collector, arg, seps, sep_id, end)) {
        pr_error("Failed to parse argument into collector definition: '%s'\n", arg);
        return NULL;
    }
    unsigned long const duration = strtoul(arg, NULL, 10);
    struct reporter *reporter = malloc(sizeof *reporter);
    if (!reporter) {
        pr_error_with_errno("Failed to allocate memory for reporter definiton '%s'", arg);
        free(collector.raw);
        return NULL;
    }
    reporter->duration_second = duration;
    reporter->next = NULL;
    reporter->type = reporter_type;
    reporter->collector = collector;
    reporter->dots_type = dots_type;
    return reporter;
}

int reporter_prepare(struct reporter *const reporter_head) {
    for (struct reporter *reporter = reporter_head; reporter; reporter = reporter->next) {
        if (collector_init(reporter->collector)) {
            pr_error("Failed to init collector for reporter type %d(%s) duration %u\n", reporter->type, reporter_get_type_string(reporter->type), reporter->duration_second);
            return 2;
        } else {
            pr_warn("Initialized collector for reporter type %d(%s) duration %u\n", reporter->type, reporter_get_type_string(reporter->type), reporter->duration_second);
        }
        reporter->dots = openvfd_lookup_dots(reporter->dots_type);
    }
    return 0;
}

int reporter_loop(struct reporter *const reporter_head, struct watcher *const watcher_head) {
    uint32_t word_last = 0;
    uint8_t dots_last = 0;
    while (true) {
        for (struct reporter *reporter = reporter_head; reporter; reporter = reporter->next) {
            unsigned remaining_second = reporter->duration_second;
            bool blink = reporter->type == REPORTER_TYPE_DATE && reporter->collector.date->blink;
            if (collector_prepare(reporter->collector)) {
                pr_error("Failed to prepare collector\n");
                return 1;
            }
            while (true) {
                sleep(1);
                char buffer[5];
                if (collector_report(reporter->collector, buffer)) {
                    pr_error("Failed to collect report\n");
                    return 2;
                }
                uint8_t dots_this = 0;
                if (watcher_head && watcher_check_all(watcher_head, &dots_this)) {
                    pr_error("Failed to get check results of all wachers\n");
                    return 3;
                }
                dots_this |= reporter->dots;
                uint32_t word_this = *(uint32_t *)buffer;
                if (blink || word_this != word_last || dots_this != dots_last) {
                    word_last = word_this;
                    dots_last = dots_this;
                    if (openvfd_write_report(word_this, dots_this, blink)) {
                        pr_error("Failed to write report to OpenVFD dev\n");
                        return 4;
                    }
                    pr_debug("Reported type %s, remaining %u seconds, report content: %s\n", reporter_get_type_string(reporter->type), remaining_second, buffer);
                } else {
                    pr_debug("Omitted report type %s, remaining %u seconds, report content: %s, since nothing changed\n", reporter_get_type_string(reporter->type), remaining_second, buffer);
                }
                if (reporter->duration_second &&  --remaining_second == 0) {
                    break;
                }
            }
            dots_last = reporter->dots;
        }
    }
}
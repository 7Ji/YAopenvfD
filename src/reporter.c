#include "reporter.h"
#include "openvfd.h"
// #include <pthread.h>

// enum reporter_type reporter_type_from_collector_type(enum collector_type const type) {
//     return (enum reporter_type) type;
// }

static const char reporter_type_strings[][7] = {
    "",
    "string",
    "temp",
    "io",
    "cpu",
    "net"
};

char const *reporter_get_type_string(enum reporter_type const type) {
    return reporter_type_strings[type];
}

enum reporter_type reporter_get_type_from_string(char const *const string) {
    if (!string || !string[0]) {
        return REPORTER_TYPE_NONE;
    }
    puts(string);
    for (enum reporter_type type = REPORTER_TYPE_NONE + 1; type <= REPORTER_TYPE_MAX; ++type) {
        if (!strcmp(reporter_type_strings[type], string)) {
            return type;
        }
    }
    return REPORTER_TYPE_NONE;
}

void reporter_all(struct reporter *reporter_head, unsigned prepare_ahead_second) {
    while (true) {
        for (struct reporter *reporter = reporter_head; reporter; reporter = reporter->next) {
            unsigned remaining_second = reporter->duration_second;
            collector_prepare(reporter->collector);
            while (true) {
                sleep(1);
                char buffer[128];
                collector_report(reporter->collector, buffer);
                openvfd_write_report(buffer);
                pr_debug("Reporting type %s, remaining %u seconds, report content: %s\n", reporter_get_type_string(reporter->type), remaining_second, reporter->report);
                if (reporter->duration_second && --remaining_second) {
                    break;
                }
            }
        }
    }
}


#define REPORTER_PARSE_ARGUMENT_MAX_LEN 128
#define REPORTER_PARSE_ARGUMENT_MAX_SEP 3

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
    char const *end = NULL;
    unsigned sep_id = 0;
    for (char const *c = arg; !end; ++c) {
        switch (*c) {
        case ':':
            if (sep_id < REPORTER_PARSE_ARGUMENT_MAX_SEP) {
                seps[sep_id++] = c;
            }
            break;
        case '\0':
            end = c;
            break;
        default:
            break;
        }
    }
    if (sep_id < 2) {
        pr_error("Argument too short, expecting at least 2 seperator: '%s'\n", arg);
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
    struct collector collector = {0};
    switch (reporter_type) {
    case REPORTER_TYPE_STRING:
        collector.type = COLLECTOR_TYPE_STRING;
        break;
    case REPORTER_TYPE_TEMP:
        collector.type = COLLECTOR_TYPE_TEMP;
        break;
    case REPORTER_TYPE_IO:
        collector.type = COLLECTOR_TYPE_IO;
        break;
    case REPORTER_TYPE_CPU:
        collector.type = COLLECTOR_TYPE_CPU;
        break;
    case REPORTER_TYPE_NET:
        collector.type = COLLECTOR_TYPE_NET;
        break;
    default:
        pr_error("Argument does not define valid reporter type: '%s'\n", arg);
        return NULL;
    }
    if (collector_parse_argument(&collector, arg, seps, sep_id, end)) {
        return NULL;
    }
    len = reporter_parse_argument_safe_len(seps[0] - arg);
    strncpy(temp, arg, len);
    temp[len] = '\0';
    unsigned long const duration = strtoul(temp, NULL, 10);
    // switch ()
    struct reporter *reporter = malloc(sizeof *reporter);
    if (!reporter) {
        free(collector.raw);
        return NULL;
    }
    reporter->duration_second = duration;
    reporter->next = NULL;
    reporter->type = reporter_type;
    // reporter->collector.type =  ;
    pr_warn("Reporter for %s duration %lu\n", reporter_get_type_string(reporter_type), duration);
    return reporter;
}
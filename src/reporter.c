#include "reporter.h"
#include "openvfd.h"

#define REPORTER_TYPE_MAX REPORTER_TYPE_NET
#define REPORTER_PARSE_ARGUMENT_MAX_LEN 128
#define REPORTER_PARSE_ARGUMENT_MAX_SEP 3

static const char reporter_type_strings[][7] = {
    "",
    "string",
    "temp",
    "io",
    "cpu",
    "net",
    "time",
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
    struct collector collector = {0};
    if (!(collector.type = (enum collector_type) reporter_type)) {
        pr_error("Argument does not define valid reporter type: '%s'\n", arg);
        return NULL;
    }
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
    return reporter;
}

int reporter_prepare(struct reporter *const reporter_head) {
    int r = openvfd_prepare();
    if (r) {
        pr_error("Failed to prepare OpenVFD: %d\n", r);
        return 1;
    }
    for (struct reporter *reporter = reporter_head; reporter; reporter = reporter->next) {
        if (collector_init(reporter->collector)) {
            pr_error("Failed to init collector for reporter type %d(%s) duration %u\n", reporter->type, reporter_get_type_string(reporter->type), reporter->duration_second);
            return 2;
        } else {
            pr_warn("Initialized collector for reporter type %d(%s) duration %u\n", reporter->type, reporter_get_type_string(reporter->type), reporter->duration_second);
        }
    }
    return 0;
}

int reporter_loop(struct reporter *reporter_head) {
    uint32_t word_last = 0;
    uint8_t dots_last = 0;
    while (true) {
        for (struct reporter *reporter = reporter_head; reporter; reporter = reporter->next) {
            unsigned remaining_second = reporter->duration_second;
            bool blink = reporter->type == REPORTER_TYPE_TIME;
            collector_prepare(reporter->collector);
            while (true) {
                sleep(1);
                char buffer[5];
                collector_report(reporter->collector, buffer);
                uint32_t word_this = *(uint32_t *)buffer;
                if (blink || word_this != word_last || reporter->dots != dots_last) {
                    word_last = word_this;
                    openvfd_write_report(word_this, reporter->dots, blink);
                    pr_debug("Reporting type %s, remaining %u seconds, report content: %s\n", reporter_get_type_string(reporter->type), remaining_second, buffer);
                } else {
                    pr_debug("Omitted report type %s, remaining %u secons, report content: %s, since nothing changed\n", reporter_get_type_string(reporter->type), remaining_second, buffer);
                }
                if (reporter->duration_second &&  --remaining_second == 0) {
                    break;
                }
            }
            dots_last = reporter->dots;
        }
    }
}
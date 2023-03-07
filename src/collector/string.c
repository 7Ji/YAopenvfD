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
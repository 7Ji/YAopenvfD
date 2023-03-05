#include "reporter.h"
#include <pthread.h>

static const char reporter_type_strings[][7] = {
    "",
    "string",
    "temp",
    "io",
    "cpu",
    "net"
};

char const *reporter_get_type_string(enum reporter_type type) {
    return reporter_type_strings[type];
}

void reporter_preparer(struct reporter *reporter_head) {

}

void reporter_all(struct reporter *reporter_head, unsigned prepare_ahead_second) {
    reporter_preparer(reporter_head);
    sleep(prepare_ahead_second);
    while (true) {
        for (struct reporter *reporter = reporter_head; reporter; reporter = reporter->next) {
            unsigned remaining_second = reporter->duration_second;
            while (true) {
                pr_debug("Reporting %s, remaining %u seconds, report content: %s\n", reporter_get_type_string(reporter->type), remaining_second, reporter->report);
                sleep(1);
                if (reporter->duration_second && --remaining_second) {
                    break;
                }
            }
        }
    }
}
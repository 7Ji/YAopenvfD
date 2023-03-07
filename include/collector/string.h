#ifndef _HAVE_COLLECTOR_STRING_H
#define _HAVE_COLLECTOR_STRING_H
#include "collector.h"
struct collector_string *collector_parse_argument_string(char const *start, char const *end);
int collector_string_prepare(struct collector_string *collector);
int collector_string_report(struct collector_string *const collector, char report[COLLECTOR_REPORT_SIZE]);
#endif
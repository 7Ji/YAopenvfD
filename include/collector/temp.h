#ifndef _HAVE_COLLECTOR_TEMP_H
#define _HAVE_COLLECTOR_TEMP_H
#include "collector.h"
struct collector_temp *collector_parse_argument_temp(char const *start, char const *end);
int collector_temp_init(struct collector_temp *collector);
int collector_temp_report(struct collector_temp *collector, char report[COLLECTOR_REPORT_SIZE]);
#endif
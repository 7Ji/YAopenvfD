#ifndef _HAVE_COLLECTOR_DATE_H
#define _HAVE_COLLECTOR_DATE_H
#include "collector.h"
struct collector_date *collector_parse_argument_date(char const *start, char const *end);
int collector_date_report(struct collector_date *collector, char report[COLLECTOR_REPORT_SIZE]);
#endif
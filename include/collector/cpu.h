#ifndef _HAVE_COLLECTOR_CPU_H
#define _HAVE_COLLECTOR_CPU_H
#include "collector.h"

struct collector_cpu *collector_parse_argument_cpu(char const *sep_type_cpuno);
int collector_cpu_init(struct collector_cpu *collector);
int collector_cpu_prepare(struct collector_cpu *collector);
int collector_cpu_report(struct collector_cpu *collector, char report[COLLECTOR_REPORT_SIZE]);
#endif
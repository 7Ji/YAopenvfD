/*
YAopenvfD/include/collector/date.h: header for logic to collect CPU usage
Copyright (C) 2023 Guoxin "7Ji" Pu <pugokushin@gmail.com>

This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with this program. If not, see <https://www.gnu.org/licenses/>.
*/

#ifndef _HAVE_COLLECTOR_CPU_H
#define _HAVE_COLLECTOR_CPU_H
#include "collector.h"

struct collector_cpu *collector_parse_argument_cpu(char const *sep_type_cpuno);
int collector_cpu_init(struct collector_cpu *collector);
int collector_cpu_prepare(struct collector_cpu *collector);
int collector_cpu_report(struct collector_cpu *collector, char report[COLLECTOR_REPORT_SIZE]);
#endif
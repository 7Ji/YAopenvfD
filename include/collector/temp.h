/*
YAopenvfD/include/collector/temp.h: header for logic to collect temperature
Copyright (C) 2023 Guoxin "7Ji" Pu <pugokushin@gmail.com>

This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with this program. If not, see <https://www.gnu.org/licenses/>.
*/

#ifndef _HAVE_COLLECTOR_TEMP_H
#define _HAVE_COLLECTOR_TEMP_H
#include "collector.h"
struct collector_temp *collector_parse_argument_temp(char const *start, char const *end);
int collector_temp_init(struct collector_temp *collector);
int collector_temp_report(struct collector_temp *collector, char report[COLLECTOR_REPORT_SIZE]);
#endif
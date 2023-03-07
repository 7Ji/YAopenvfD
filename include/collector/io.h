/*
YAopenvfD/include/collector/io.h: header for logic to collect I/O usage
Copyright (C) 2023 Guoxin "7Ji" Pu <pugokushin@gmail.com>

This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with this program. If not, see <https://www.gnu.org/licenses/>.
*/

#ifndef _HAVE_COLLECTOR_IO_H
#define _HAVE_COLLECTOR_IO_H
#include "collector.h"

struct collector_io *collector_parse_argument_io(char const *sep_type_blkdev, char const *sep_blkdev_rw, char const *end);
int collector_io_init(struct collector_io *collector);
int collector_io_prepare(struct collector_io *collector);
int collector_io_report(struct collector_io *collector, char report[COLLECTOR_REPORT_SIZE]);
#endif
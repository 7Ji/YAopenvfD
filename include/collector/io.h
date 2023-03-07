#ifndef _HAVE_COLLECTOR_IO_H
#define _HAVE_COLLECTOR_IO_H
#include "collector.h"

struct collector_io *collector_parse_argument_io(char const *sep_type_blkdev, char const *sep_blkdev_rw, char const *end);
int collector_io_init(struct collector_io *collector);
int collector_io_prepare(struct collector_io *collector);
int collector_io_report(struct collector_io *collector, char report[COLLECTOR_REPORT_SIZE]);
#endif
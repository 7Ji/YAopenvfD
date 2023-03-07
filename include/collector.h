/*
YAopenvfD/include/collector.h: header for logic to collect data
Copyright (C) 2023 Guoxin "7Ji" Pu <pugokushin@gmail.com>

This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with this program. If not, see <https://www.gnu.org/licenses/>.
*/

#ifndef _HAVE_WORKER_H
#define _HAVE_WORKER_H
#include "common.h"

#define COLLECTOR_REPORT_SIZE   5

enum collector_type {
    COLLECTOR_TYPE_NONE,
    COLLECTOR_TYPE_STRING,
    COLLECTOR_TYPE_TEMP,
    COLLECTOR_TYPE_IO,
    COLLECTOR_TYPE_CPU,
    COLLECTOR_TYPE_NET,
    COLLECTOR_TYPE_DATE
};

struct collector_string {
    char string[256];
    size_t len;
    bool loop;
    unsigned off;
};

struct collector_temp {
    unsigned short zone;
    int stat_fd;

};

enum collector_io_type {
    COLLECTOR_IO_TYPE_READ,
    COLLECTOR_IO_TYPE_WRITE,
    COLLECTOR_IO_TYPE_MIXED
};

struct collector_io {
    char blkdev[NAME_MAX + 1];
    int stat_fd;
    size_t read_sectors_last;
    size_t read_sectors_this;
    size_t write_sectors_last;
    size_t write_sectors_this;
    enum collector_io_type type;
};

struct collector_cpu {
    int cpu_no;
    int stat_fd;
    char label[16];
    char *buffer;
    size_t alloc;
    unsigned long busy_last;
    unsigned long busy_this;
    unsigned long idle_last;
    unsigned long idle_this;
};

enum collector_net_type {
    COLLECTOR_NET_TYPE_TX,
    COLLECTOR_NET_TYPE_RX,
    COLLECTOR_NET_TYPE_MIXED
};

struct collector_net {
    char interface[NAME_MAX + 1];
    int tx_fd;
    int rx_fd;
    enum collector_net_type type;
    bool tx_parse;
    bool rx_parse;
    unsigned long tx_last;
    unsigned long tx_this;
    unsigned long rx_last;
    unsigned long rx_this;
};

enum collector_date_type {
    COLLECTOR_DATE_TYPE_NONE,
    COLLECTOR_DATE_TYPE_24H,
    COLLECTOR_DATE_TYPE_12H,
    COLLECTOR_DATE_TYPE_YEAR,
    COLLECTOR_DATE_TYPE_YEAR_MONTH,
    COLLECTOR_DATE_TYPE_MONTH_YEAR,
    COLLECTOR_DATE_TYPE_MONTH_DAY,
    COLLECTOR_DATE_TYPE_DAY_MONTH,
    COLLECTOR_DATE_TYPE_WEEKDAY
};

struct collector_date {
    enum collector_date_type type;
    bool blink;
};

struct collector {
    enum collector_type type;
    union {
        void *raw;
        struct collector_string *string;
        struct collector_temp *temp;
        struct collector_io *io;
        struct collector_cpu *cpu;
        struct collector_net *net;
        struct collector_date *date;
    };
};

int collector_init(struct collector collector);
int collector_prepare(struct collector collector);
int collector_report(struct collector collector, char report[5]);
int collector_parse_argument(struct collector *collector, char const *const arg, char const *const seps[], unsigned const sep_count, char const *end);

#endif
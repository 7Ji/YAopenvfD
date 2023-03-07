#ifndef _HAVE_WORKER_H
#define _HAVE_WORKER_H
#include "common.h"
// #include "reporter.h"

#define COLLECTOR_IO_DISKSTAT "/proc/diskstats"
#define COLLECTOR_IO_ALLOC_BASE 0x1000
#define COLLECTOR_IO_ALLOC_MULTIPLY 2

#define COLLECTOR_DEFINER_IO_BLKDEV_MAXLEN  64

#define COLLECTOR_IO_ID_NAME    3
#define COLLECTOR_IO_ID_READ    6
#define COLLECTOR_IO_ID_WRITE   10

enum collector_type {
    COLLECTOR_TYPE_NONE,
    COLLECTOR_TYPE_STRING,
    COLLECTOR_TYPE_TEMP,
    COLLECTOR_TYPE_IO,
    COLLECTOR_TYPE_CPU,
    COLLECTOR_TYPE_NET,
    COLLECTOR_TYPE_TIME,
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
    };
};

// int collector_io(struct collector_io *collector);

// enum collector_type collector_type_from_reporter_type(enum reporter_type type);
int collector_init(struct collector collector);
int collector_prepare(struct collector collector);
int collector_report(struct collector collector, char report[5]);
int collector_parse_argument(struct collector *collector, char const *const arg, char const *const seps[], unsigned const sep_count, char const *end);

#endif
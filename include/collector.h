#ifndef _HAVE_WORKER_H
#define _HAVE_WORKER_H
#include "common.h"


#define COLLECTOR_IO_DISKSTAT "/proc/diskstats"
#define COLLECTOR_IO_ALLOC_BASE 0x1000
#define COLLECTOR_IO_ALLOC_MULTIPLY 2

#define COLLECTOR_DEFINER_IO_BLKDEV_MAXLEN  64

#define COLLECTOR_IO_ID_NAME    3
#define COLLECTOR_IO_ID_READ    6
#define COLLECTOR_IO_ID_WRITE   10

// enum collector_type {
//     COLLECTOR_TYPE_NONE,
//     COLLECTOR_TYPE_STRING,
//     COLLECTOR_TYPE_TEMP,
//     COLLECTOR_TYPE_IO,
//     COLLECTOR_TYPE_CPU,
//     COLLECTOR_TYPE_NET
// };

struct collector {
    struct collector *next;
};

struct collector_string {
    struct collector_string *next;
    char string[256];
};

struct collector_temp {
    struct collector_temp *next;
    unsigned short zone;
};

struct collector_io {
    struct collector_io *next;
    char blkdev[64];
    size_t read_sectors;
    size_t write_sectors;
    size_t mixed_sectors;
};

struct collector_cpu {
    struct collector_cpu *next;
    
};

struct collector_net {
    struct collector_net *next;
    char interface[64];
};

// union collector_definer {
//     struct collector_string *string;
//     struct collector_temp *temp;
//     struct collector_io *io;
//     struct collector_cpu *cpu;
//     struct collector_net *net;
// };

struct collectors {
    struct collector_string *string;
    struct collector_temp *temp;
    struct collector_io *io;
    struct collector_cpu *cpu;
    struct collector_net *net;
    char report[5];
};

// struct collector {
//     enum collector_type type;
//     union collector_definer definer;
//     // union collector_storage storage;
//     char report[5];
// };

int collector_io(struct collector_io *collector);

#endif
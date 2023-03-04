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

enum collector_type {
    COLLECTOR_TYPE_STRING,
    COLLECTOR_TYPE_TEMP,
    COLLECTOR_TYPE_IO,
    COLLECTOR_TYPE_CPU,
    COLLECTOR_TYPE_NET
};

struct collector_definer_string {
    char string[256];
};

struct collector_definer_temp {
    unsigned short zone;
};

struct collector_definer_io {
    char blkdev[64];
    struct collector_definer_io *next;
};

struct collector_definer_cpu {

};

struct collector_definer_net {

};

union collector_definer {
    struct collector_definer_string *string;
    struct collector_definer_temp *temp;
    struct collector_definer_io *io;
    struct collector_definer_cpu *cpu;
    struct collector_definer_net *net;
};

struct collector_storage_io {
    size_t read, write, mixed;
};

union collector_storage {
    struct collector_storage_io *io;
};

struct collector {
    enum collector_type type;
    union collector_definer definer;
    union collector_storage storage;
    char report[5];
};

int collector_io(struct collector *collector);

#endif
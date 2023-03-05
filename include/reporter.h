#ifndef _HAVE_REPORTER_H
#define _HAVE_REPORTER_H
#include "common.h"
#include "collector.h"
enum reporter_type {
    REPORTER_TYPE_NONE,
    REPORTER_TYPE_STRING,
    REPORTER_TYPE_TEMP,
    REPORTER_TYPE_IO,
    REPORTER_TYPE_CPU,
    REPORTER_TYPE_NET
};

struct reporter {
    struct reporter *next;
    enum reporter_type type;
    unsigned duration_second;
    char *report;
};

char const *reporter_get_type_string(enum reporter_type type);
#endif
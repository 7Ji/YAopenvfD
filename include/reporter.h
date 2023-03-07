#ifndef _HAVE_REPORTER_H
#define _HAVE_REPORTER_H
#include "common.h"
#include "collector.h"
#include "dots.h"
#include "watcher.h"

enum reporter_type {
    REPORTER_TYPE_NONE,
    REPORTER_TYPE_STRING,
    REPORTER_TYPE_TEMP,
    REPORTER_TYPE_IO,
    REPORTER_TYPE_CPU,
    REPORTER_TYPE_NET,
    REPORTER_TYPE_DATE
};

struct reporter {
    struct reporter *next;
    enum reporter_type type;
    struct collector collector;
    unsigned duration_second;
    enum dots_type dots_type;
    uint8_t dots;
};

struct reporter *reporter_parse_argument(char const *arg);
int reporter_prepare(struct reporter *reporter_head);
int reporter_loop(struct reporter *reporter_head, struct watcher *watcher_head);
#endif
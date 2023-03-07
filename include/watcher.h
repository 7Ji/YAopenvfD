#ifndef _HAVE_WATCHER_H
#define _HAVE_WATCHER_H
#include "common.h"
#include "dots.h"

enum watcher_type {
    WATCHER_TYPE_NONE,
    WATCHER_TYPE_FILE,
    WATCHER_TYPE_DEV,
    WATCHER_TYPE_SYS,
    WATCHER_TYPE_NET_CARRIER
};

struct watcher_file {
    char path[PATH_MAX];
};

struct watcher_net_carrier {
    char interface[16];
    int carrier_fd;
};

struct watcher {
    struct watcher *next;
    enum watcher_type type;
    enum dots_type dots_type;
    uint8_t dots_mask;
    union {
        void *raw;
        struct watcher_file *file;
        struct watcher_net_carrier *net_carrier;
    };
};

struct watcher *wacher_parse_argument(char const *arg);
int watcher_prepare(struct watcher *watcher_head);
int watcher_check_all(struct watcher *const watcher_head, uint8_t *dots);
#endif
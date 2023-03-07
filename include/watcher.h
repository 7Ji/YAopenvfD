/*
YAopenvfD/include/watcher.h: header for logic to watch for hotplug events
Copyright (C) 2023 Guoxin "7Ji" Pu <pugokushin@gmail.com>

This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with this program. If not, see <https://www.gnu.org/licenses/>.
*/

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
/*
YAopenvfD/include/reporter.h: header for logic to report data
Copyright (C) 2023 Guoxin "7Ji" Pu <pugokushin@gmail.com>

This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with this program. If not, see <https://www.gnu.org/licenses/>.
*/

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
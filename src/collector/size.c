/*
YAopenvfD/src/collector/size.c: logic to convert size to human-readable format
Copyright (C) 2023 Guoxin "7Ji" Pu <pugokushin@gmail.com>

This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with this program. If not, see <https://www.gnu.org/licenses/>.
*/

#include "collector/size.h"

#define COLLECTOR_SIZE_SUFFIXES   "BKMGTPEZY"
static const unsigned short collector_len_size_suffixes = strlen(COLLECTOR_SIZE_SUFFIXES);
static const char collector_size_suffixes[] = COLLECTOR_SIZE_SUFFIXES;

size_t collector_size_to_human_readable(size_t size, char *suffix) {
    for (unsigned short i = 0; i < collector_len_size_suffixes; ++i) {
        if (size < 0x400) {
            *suffix = collector_size_suffixes[i];
            return size;
        }
        size /= 0x400;
    }
    *suffix = 'B';
    return size;
}
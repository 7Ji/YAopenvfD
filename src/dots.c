/*
YAopenvfD/src/watcher.c: logic to find/convert LED dots
Copyright (C) 2023 Guoxin "7Ji" Pu <pugokushin@gmail.com>

This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with this program. If not, see <https://www.gnu.org/licenses/>.
*/

#include "dots.h"

#define DOTS_TYPE_MAX   DOTS_TYPE_AV

#define DOTS_SETUP(TYPE, BIT) \
    [DOTS_TYPE_ ##TYPE] = BIT

char const dots_type_strings[][7] = {
    "",
    "alarm",
    "usb",
    "play",
    "pause",
    "sec",
    "eth",
    "wifi",
    "apps",
    "setup",
    "card",
    "hdmi",
    "cvbs",
    "power",
    "lan",
    "wifihi",
    "wifilo",
    "bt",
    "spdif",
    "av"
};

uint8_t dots_bit_map_table[] = {
    0,
    1,
    2,
    3,
    4,
    5,
    6,
    7
};

uint8_t const dots_lookup_table[][DOTS_TYPE_AV+1] = {
    {
        DOTS_SETUP(ALARM, 0),
        DOTS_SETUP(USB, 1),
        DOTS_SETUP(PLAY, 2),
        DOTS_SETUP(PAUSE, 3),
        DOTS_SETUP(SEC, 4),
        DOTS_SETUP(ETH, 5),
        DOTS_SETUP(WIFI, 6)
    },
    {
        DOTS_SETUP(APPS, 0),
        DOTS_SETUP(SETUP, 1),
        DOTS_SETUP(USB, 2),
        DOTS_SETUP(CARD, 3),
        DOTS_SETUP(SEC, 4),
        DOTS_SETUP(HDMI, 5),
        DOTS_SETUP(CVBS, 6)
    },
    {
        DOTS_SETUP(POWER, 2),
        DOTS_SETUP(LAN, 3),
        DOTS_SETUP(SEC, 4),
        DOTS_SETUP(WIFIHI, 5),
        DOTS_SETUP(WIFILO, 6)
    },
    {
        DOTS_SETUP(BT, 0),
        DOTS_SETUP(ETH, 1),
        DOTS_SETUP(WIFI, 2),
        DOTS_SETUP(SPDIF, 3),
        DOTS_SETUP(SEC, 4),
        DOTS_SETUP(HDMI, 5),
        DOTS_SETUP(AV, 6)
    }
};

int dots_set_order(char const *const arg) {
    if (!arg || !arg[0]) {
        pr_error("String not valid");
        return 1;
    }
    char const *part = arg;
    unsigned long bit = 0;
    unsigned id = 0;
    for (char const *c = arg;; ++c) {
        switch (*c) {
        case '0'...'9':
            break;
        case ',':
            if ((bit = strtoul(part, NULL, 10)) > 7) {
                pr_warn("Dots bit order too large: %lu\n", bit);
                return 2;
            }
            dots_bit_map_table[id++] = bit;
            part = c + 1;
            break;
        case '\0':
            pr_warn("Dots bit order overwrite: %hu, %hu, %hu, %hu, %hu, %hu, %hu, %hu\n", dots_bit_map_table[0], dots_bit_map_table[1], dots_bit_map_table[2], dots_bit_map_table[3], dots_bit_map_table[4], dots_bit_map_table[5], dots_bit_map_table[6], dots_bit_map_table[7]);
            return 0;
        default:
            pr_error("Invalid character in argument: '%c' in '%s'\n", *c, arg);
            return 2;
        }
    }
}

enum dots_type dots_get_type_from_string(char const *const string) {
    if (!string || !string[0]) {
        return DOTS_TYPE_NONE;
    }
    for (enum dots_type type = DOTS_TYPE_NONE + 1; type <= DOTS_TYPE_MAX; ++type) {
        if (!strcmp(dots_type_strings[type], string)) {
            return type;
        }
    }
    return DOTS_TYPE_NONE;
}

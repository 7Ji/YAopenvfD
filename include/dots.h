/*
YAopenvfD/include/dots.h: header for logic to parse LED dots
Copyright (C) 2023 Guoxin "7Ji" Pu <pugokushin@gmail.com>

This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with this program. If not, see <https://www.gnu.org/licenses/>.
*/

#ifndef _HAVE_DOTS_H
#define _HAVE_DOTS_H
#include "common.h"

enum dots_type {
    DOTS_TYPE_NONE,
    DOTS_TYPE_ALARM,
    DOTS_TYPE_USB,
    DOTS_TYPE_PLAY,
    DOTS_TYPE_PAUSE,
    DOTS_TYPE_SEC,
    DOTS_TYPE_ETH,
    DOTS_TYPE_WIFI,
    DOTS_TYPE_APPS,
    DOTS_TYPE_SETUP,
    DOTS_TYPE_CARD,
    DOTS_TYPE_HDMI,
    DOTS_TYPE_CVBS,
    DOTS_TYPE_POWER,
    DOTS_TYPE_LAN,
    DOTS_TYPE_WIFIHI,
    DOTS_TYPE_WIFILO,
    DOTS_TYPE_BT,
    DOTS_TYPE_SPDIF,
    DOTS_TYPE_AV
};

// extern char const dots_type_strings[][10];
extern uint8_t dots_bit_map_table[];
extern uint8_t const dots_lookup_table[][DOTS_TYPE_AV+1];
enum dots_type dots_get_type_from_string(char const *string);
int dots_set_order(char const *arg);
#endif
/*
YAopenvfD/include/openvfd.h: header for logic to interact with openvfd
Copyright (C) 2023 Guoxin "7Ji" Pu <pugokushin@gmail.com>

This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with this program. If not, see <https://www.gnu.org/licenses/>.
*/

#ifndef _HAVE_OPENVFD_H
#define _HAVE_OPENVFD_H
#include "common.h"
#include "dots.h"

int openvfd_prepare();
uint8_t openvfd_lookup_dots(enum dots_type type);
int openvfd_write_report(uint32_t word, uint8_t dots, bool const blink);

#endif

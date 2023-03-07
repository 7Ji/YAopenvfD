/*
YAopenvfD/include/glyphs/positions.h: positions for glyphs for characters for 7-segment LED display
Copyright (C) 2023 Guoxin "7Ji" Pu <pugokushin@gmail.com>

This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with this program. If not, see <https://www.gnu.org/licenses/>.
*/

#ifndef _HAVE_GLYPHS_POSITIONS_H
#define _HAVE_GLYPHS_POSITIONS_H
#include "glyphs/displays.h"
#define GLYPHS_BOTTOM(X)    GLYPHS_ ## X ## _A
#define GLYPHS_LEFT_DOWN(X) GLYPHS_ ## X ## _B
#define GLYPHS_LEFT_UP(X)   GLYPHS_ ## X ## _C
#define GLYPHS_TOP(X)       GLYPHS_ ## X ## _D
#define GLYPHS_RIGHT_UP(X)  GLYPHS_ ## X ## _E
#define GLYPHS_RIGHT_DOWN(X)GLYPHS_ ## X ## _F
#define GLYPHS_MIDDLE(X)    GLYPHS_ ## X ## _G
#define GLYPHS_POINT(X)     GLYPHS_ ## X ## _P
#endif
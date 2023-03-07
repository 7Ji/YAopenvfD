/*
YAopenvfD/include/glyphs/char/puntuations.h: glyphs for punctuation characters for 7-segment LED display
Copyright (C) 2023 Guoxin "7Ji" Pu <pugokushin@gmail.com>

This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with this program. If not, see <https://www.gnu.org/licenses/>.
*/

#ifndef _HAVE_GLYPHS_CHAR_PUNCTUATIONS_H
#define _HAVE_GLYPHS_CHAR_PUNCTUATIONS_H
#include "glyphs/positions.h"
#define GLYPHS_CHAR_SLASH(X) \
                            GLYPHS_RIGHT_UP(X) | GLYPHS_MIDDLE(X) | GLYPHS_LEFT_DOWN(X)
#define GLYPHS_CHAR_BACKSLASH(X) \
                            GLYPHS_LEFT_UP(X) | GLYPHS_MIDDLE(X) | GLYPHS_RIGHT_DOWN(X)
#define GLYPHS_CHAR_PERCENT(X) \
                            GLYPHS_RIGHT_UP(X) | GLYPHS_MIDDLE(X) | GLYPHS_LEFT_DOWN(X)
#define GLYPHS_CHAR_HYPHEN_MINUS(X) \
                            GLYPHS_MIDDLE(X)
#define GLYPHS_CHAR_LOW_LINE(X) \
                            GLYPHS_BOTTOM(X)
#define GLYPHS_CHAR_DEGREE(X) \
                            GLYPHS_LEFT_UP(X) | GLYPHS_TOP(X) | GLYPHS_RIGHT_UP(X) | GLYPHS_MIDDLE(X)
#endif
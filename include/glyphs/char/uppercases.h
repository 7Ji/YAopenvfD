/*
YAopenvfD/include/glyphs/char/uppercases.h: glyphs for uppercase characters for 7-segment LED display
Copyright (C) 2023 Guoxin "7Ji" Pu <pugokushin@gmail.com>

This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with this program. If not, see <https://www.gnu.org/licenses/>.
*/

#ifndef _HAVE_GLYPHS_CHAR_UPPERCASES_H
#define _HAVE_GLYPHS_CHAR_UPPERCASES_H
#include "glyphs/positions.h"
/* Most of the below representations are adapted from https://en.wikipedia.org/wiki/Seven-segment_display_character_representations#Alphabet */
/* _
  |_|
  | | */
#define GLYPHS_CHAR_UPPER_A(X)  GLYPHS_LEFT_UP(X) | GLYPHS_LEFT_DOWN(X) | GLYPHS_TOP(X) | GLYPHS_RIGHT_UP(X) | GLYPHS_RIGHT_DOWN(X) | GLYPHS_MIDDLE(X)
/* _
  |_|
  |_| */
#define GLYPHS_CHAR_UPPER_B(X)  GLYPHS_LEFT_UP(X) | GLYPHS_LEFT_DOWN(X) | GLYPHS_TOP(X) | GLYPHS_RIGHT_UP(X) | GLYPHS_MIDDLE(X) | GLYPHS_RIGHT_DOWN(X) | GLYPHS_BOTTOM(X)
/* _
  |
  |_ */
#define GLYPHS_CHAR_UPPER_C(X)  GLYPHS_TOP(X) | GLYPHS_LEFT_UP(X) | GLYPHS_LEFT_DOWN(X) | GLYPHS_BOTTOM(X)
/* _
    |
  |_| */
#define GLYPHS_CHAR_UPPER_D(X)  GLYPHS_LEFT_DOWN(X) | GLYPHS_TOP(X) | GLYPHS_RIGHT_UP(X) | GLYPHS_RIGHT_DOWN(X) | GLYPHS_BOTTOM(X)
/* _
  |_
  |_ */
#define GLYPHS_CHAR_UPPER_E(X)  GLYPHS_LEFT_UP(X) | GLYPHS_LEFT_DOWN(X) | GLYPHS_TOP(X) | GLYPHS_MIDDLE(X) | GLYPHS_BOTTOM(X)
/* _
  |_
  |  */
#define GLYPHS_CHAR_UPPER_F(X)  GLYPHS_LEFT_UP(X) | GLYPHS_LEFT_DOWN(X) | GLYPHS_TOP(X) | GLYPHS_MIDDLE(X)
/* _
  |
  |_| */
#define GLYPHS_CHAR_UPPER_G(X)  GLYPHS_TOP(X) | GLYPHS_LEFT_UP(X) | GLYPHS_LEFT_DOWN(X) | GLYPHS_BOTTOM(X) | GLYPHS_RIGHT_DOWN(X)
/*
  |_| 
  | | */
#define GLYPHS_CHAR_UPPER_H(X)  GLYPHS_LEFT_UP(X) | GLYPHS_LEFT_DOWN(X) | GLYPHS_MIDDLE(X) | GLYPHS_RIGHT_UP(X) | GLYPHS_RIGHT_DOWN(X)
/*
  |
  |   */
#define GLYPHS_CHAR_UPPER_I(X)  GLYPHS_LEFT_UP(X) | GLYPHS_LEFT_DOWN(X)
/*
    |
   _| */
#define GLYPHS_CHAR_UPPER_J(X)  GLYPHS_RIGHT_UP(X) | GLYPHS_RIGHT_DOWN(X) | GLYPHS_BOTTOM(X)
/* _
  |_
  | | */
#define GLYPHS_CHAR_UPPER_K(X)  GLYPHS_LEFT_UP(X) | GLYPHS_LEFT_DOWN(X) | GLYPHS_TOP(X) | GLYPHS_MIDDLE(X) | GLYPHS_RIGHT_DOWN(X)
/*
  |
  |_ */
#define GLYPHS_CHAR_UPPER_L(X)  GLYPHS_LEFT_UP(X) | GLYPHS_LEFT_DOWN(X) | GLYPHS_BOTTOM(X)
/* _
  | |
   _  */
#define GLYPHS_CHAR_UPPER_M(X)  GLYPHS_LEFT_UP(X) | GLYPHS_TOP(X) | GLYPHS_BOTTOM(X) | GLYPHS_RIGHT_UP(X)
/* _ 
  | |
  | | */
#define GLYPHS_CHAR_UPPER_N(X)  GLYPHS_LEFT_UP(X) | GLYPHS_LEFT_DOWN(X) | GLYPHS_TOP(X) | GLYPHS_RIGHT_UP(X) | GLYPHS_RIGHT_DOWN(X)
/* _ 
  | |
  |_| */
#define GLYPHS_CHAR_UPPER_O(X)  GLYPHS_LEFT_UP(X) | GLYPHS_LEFT_DOWN(X) | GLYPHS_BOTTOM(X) | GLYPHS_RIGHT_DOWN(X) | GLYPHS_RIGHT_UP(X) | GLYPHS_TOP(X)
/* _
  |_|
  |   */
#define GLYPHS_CHAR_UPPER_P(X)  GLYPHS_LEFT_UP(X) | GLYPHS_LEFT_DOWN(X) | GLYPHS_TOP(X) | GLYPHS_RIGHT_UP(X) | GLYPHS_MIDDLE(X)
/* _
  |_|
   _ */
#define GLYPHS_CHAR_UPPER_Q(X)  GLYPHS_TOP(X) | GLYPHS_LEFT_UP(X) | GLYPHS_MIDDLE(X) | GLYPHS_RIGHT_UP(X) | GLYPHS_BOTTOM(X)
/* _
  |_|
  |_  */
#define GLYPHS_CHAR_UPPER_R(X)  GLYPHS_LEFT_UP(X) | GLYPHS_LEFT_DOWN(X) | GLYPHS_TOP(X) | GLYPHS_RIGHT_UP(X) | GLYPHS_MIDDLE(X) | GLYPHS_BOTTOM(X)
/* _
  |_
   _|*/
#define GLYPHS_CHAR_UPPER_S(X)  GLYPHS_TOP(X) | GLYPHS_LEFT_UP(X) | GLYPHS_MIDDLE(X) | GLYPHS_RIGHT_DOWN(X) | GLYPHS_BOTTOM(X)
/* _
  |
  |  */
#define GLYPHS_CHAR_UPPER_T(X)  GLYPHS_TOP(X) | GLYPHS_LEFT_UP(X) | GLYPHS_LEFT_DOWN(X)
/*
  | |
  |_| */
#define GLYPHS_CHAR_UPPER_U(X)  GLYPHS_LEFT_UP(X) | GLYPHS_LEFT_DOWN(X) | GLYPHS_BOTTOM(X) | GLYPHS_RIGHT_DOWN(X) | GLYPHS_RIGHT_UP(X)
/*
  | |
   _| */
#define GLYPHS_CHAR_UPPER_V(X)  GLYPHS_LEFT_UP(X) | GLYPHS_BOTTOM(X) | GLYPHS_RIGHT_DOWN(X) | GLYPHS_RIGHT_UP(X)
/* _
  
  |_| */
#define GLYPHS_CHAR_UPPER_W(X)  GLYPHS_LEFT_DOWN(X) | GLYPHS_BOTTOM(X) | GLYPHS_TOP(X) | GLYPHS_RIGHT_DOWN(X)
/* _
   _
   _ */
#define GLYPHS_CHAR_UPPER_X(X)  GLYPHS_TOP(X) | GLYPHS_MIDDLE(X) | GLYPHS_BOTTOM(X)
/* 
  |_|
   _ */
#define GLYPHS_CHAR_UPPER_Y(X)  GLYPHS_LEFT_UP(X) | GLYPHS_MIDDLE(X) | GLYPHS_RIGHT_UP(X) | GLYPHS_BOTTOM(X)
/* _
   _|
  |_ */
#define GLYPHS_CHAR_UPPER_Z(X)  GLYPHS_TOP(X) | GLYPHS_RIGHT_UP(X) | GLYPHS_MIDDLE(X) | GLYPHS_LEFT_DOWN(X) | GLYPHS_BOTTOM(X)
#endif
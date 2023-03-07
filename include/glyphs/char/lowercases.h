/*
YAopenvfD/include/glyphs/char/lowercases.h: glyphs for lowercase characters for 7-segment LED display
Copyright (C) 2023 Guoxin "7Ji" Pu <pugokushin@gmail.com>

This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with this program. If not, see <https://www.gnu.org/licenses/>.
*/

#ifndef _HAVE_GLYPHS_CHAR_LOWERCASES_H
#define _HAVE_GLYPHS_CHAR_LOWERCASES_H
#include "glyphs/positions.h"
/* Most of the below representations are adapted from https://en.wikipedia.org/wiki/Seven-segment_display_character_representations#Alphabet */
/* 
   _
   _| */
#define GLYPHS_CHAR_LOWER_A(X)  GLYPHS_MIDDLE(X) | GLYPHS_BOTTOM(X) | GLYPHS_RIGHT_DOWN(X)
/*
  |_
  |_| */
#define GLYPHS_CHAR_LOWER_B(X)  GLYPHS_LEFT_UP(X) | GLYPHS_LEFT_DOWN(X) | GLYPHS_MIDDLE(X) | GLYPHS_RIGHT_DOWN(X) | GLYPHS_BOTTOM(X)
/*
   _
  |_ */
#define GLYPHS_CHAR_LOWER_C(X)  GLYPHS_MIDDLE(X) | GLYPHS_LEFT_DOWN(X) | GLYPHS_BOTTOM(X)
/*
   _|
  |_| */
#define GLYPHS_CHAR_LOWER_D(X)  GLYPHS_MIDDLE(X) | GLYPHS_LEFT_DOWN(X) | GLYPHS_BOTTOM(X) | GLYPHS_RIGHT_UP(X) | GLYPHS_RIGHT_DOWN(X)
/*

  |_ */
#define GLYPHS_CHAR_LOWER_E(X)  GLYPHS_LEFT_DOWN(X) | GLYPHS_BOTTOM(X)
/*
  |_
  |  */
#define GLYPHS_CHAR_LOWER_F(X)  GLYPHS_LEFT_UP(X) | GLYPHS_LEFT_DOWN(X) | GLYPHS_MIDDLE(X)
/* _
   _
  |_  */
#define GLYPHS_CHAR_LOWER_G(X)  GLYPHS_TOP(X) | GLYPHS_MIDDLE(X) | GLYPHS_BOTTOM(X) | GLYPHS_LEFT_DOWN(X)
/*
  |_
  | | */
#define GLYPHS_CHAR_LOWER_H(X)  GLYPHS_LEFT_UP(X) | GLYPHS_LEFT_DOWN(X) | GLYPHS_MIDDLE(X) | GLYPHS_RIGHT_DOWN(X)
/* _
  
  |_ */
#define GLYPHS_CHAR_LOWER_I(X)  GLYPHS_LEFT_DOWN(X) | GLYPHS_BOTTOM(X) | GLYPHS_TOP(X)
/* _

   _| */
#define GLYPHS_CHAR_LOWER_J(X)  GLYPHS_RIGHT_DOWN(X) | GLYPHS_BOTTOM(X) | GLYPHS_TOP(X)
/* _
  |_
   _ */ 
#define GLYPHS_CHAR_LOWER_K(X)  GLYPHS_TOP(X) | GLYPHS_LEFT_UP(X) | GLYPHS_MIDDLE(X) | GLYPHS_BOTTOM(X)
/*
  |
  |   */
#define GLYPHS_CHAR_LOWER_L(X)  GLYPHS_LEFT_UP(X) | GLYPHS_LEFT_DOWN(X)
/* _
   _
  | | */
#define GLYPHS_CHAR_LOWER_M(X)  GLYPHS_TOP(X) | GLYPHS_LEFT_DOWN(X) | GLYPHS_MIDDLE(X) | GLYPHS_RIGHT_DOWN(X)
/*
   _
  | | */
#define GLYPHS_CHAR_LOWER_N(X)  GLYPHS_LEFT_DOWN(X) | GLYPHS_MIDDLE(X) | GLYPHS_RIGHT_DOWN(X)
/*
   _
  |_| */
#define GLYPHS_CHAR_LOWER_O(X)  GLYPHS_MIDDLE(X) | GLYPHS_LEFT_DOWN(X) | GLYPHS_BOTTOM(X) | GLYPHS_RIGHT_DOWN(X)
/* _
  |_|
  |   */
#define GLYPHS_CHAR_LOWER_P(X)  GLYPHS_LEFT_UP(X) | GLYPHS_LEFT_DOWN(X) | GLYPHS_TOP(X) | GLYPHS_RIGHT_UP(X) | GLYPHS_MIDDLE(X)
/* _
  |_|
    | */
#define GLYPHS_CHAR_LOWER_Q(X)  GLYPHS_TOP(X) | GLYPHS_LEFT_UP(X) | GLYPHS_MIDDLE(X) | GLYPHS_RIGHT_UP(X) | GLYPHS_RIGHT_DOWN(X)
/*
   _
  |  */
#define GLYPHS_CHAR_LOWER_R(X)  GLYPHS_LEFT_DOWN(X) | GLYPHS_MIDDLE(X)
/*

   _| */
#define GLYPHS_CHAR_LOWER_S(X)  GLYPHS_RIGHT_DOWN(X) | GLYPHS_BOTTOM(X)
/*
  |_
  |_  */
#define GLYPHS_CHAR_LOWER_T(X)  GLYPHS_MIDDLE(X) | GLYPHS_LEFT_UP(X) | GLYPHS_LEFT_DOWN(X) | GLYPHS_BOTTOM(X)
/*

  |_| */
#define GLYPHS_CHAR_LOWER_U(X)  GLYPHS_LEFT_DOWN(X) | GLYPHS_BOTTOM(X) | GLYPHS_RIGHT_DOWN(X)
/*

   _| */
#define GLYPHS_CHAR_LOWER_V(X)  GLYPHS_BOTTOM(X) | GLYPHS_RIGHT_DOWN(X)
/*
  | |
   _  */
#define GLYPHS_CHAR_LOWER_W(X)  GLYPHS_LEFT_UP(X) | GLYPHS_BOTTOM(X) | GLYPHS_RIGHT_UP(X)
/*
   _
   _  */
#define GLYPHS_CHAR_LOWER_X(X)  GLYPHS_MIDDLE(X) | GLYPHS_BOTTOM(X)
/*
  |_|
   _| */
#define GLYPHS_CHAR_LOWER_Y(X)  GLYPHS_LEFT_UP(X) | GLYPHS_MIDDLE(X) | GLYPHS_RIGHT_UP(X) | GLYPHS_RIGHT_DOWN(X) | GLYPHS_BOTTOM(X)
/*
   _
   _  */
#define GLYPHS_CHAR_LOWER_Z(X)  GLYPHS_MIDDLE(X) | GLYPHS_BOTTOM(X)
#endif
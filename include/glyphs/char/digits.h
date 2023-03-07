#ifndef _HAVE_GLYPHS_CHAR_DIGITS_H
#define _HAVE_GLYPHS_CHAR_DIGITS_H
#include "glyphs/positions.h"

/* Most of the below representations are adapted from https://en.wikipedia.org/wiki/Seven-segment_display_character_representations#Alphabet */
/* _
  | |
  |_| */
#define GLYPHS_CHAR_0(X)    GLYPHS_LEFT_UP(X) | GLYPHS_LEFT_DOWN(X) | GLYPHS_BOTTOM(X) | GLYPHS_RIGHT_DOWN(X) | GLYPHS_RIGHT_UP(X) | GLYPHS_TOP(X)
/*
    |
    | */
#define GLYPHS_CHAR_1(X)    GLYPHS_RIGHT_UP(X) | GLYPHS_RIGHT_DOWN(X)
/* _
   _|
  |_ */
#define GLYPHS_CHAR_2(X)    GLYPHS_TOP(X) | GLYPHS_RIGHT_UP(X) | GLYPHS_MIDDLE(X) | GLYPHS_LEFT_DOWN(X) | GLYPHS_BOTTOM(X)
/* _
   _|
   _| */
#define GLYPHS_CHAR_3(X)    GLYPHS_TOP(X) | GLYPHS_RIGHT_UP(X) | GLYPHS_MIDDLE(X) | GLYPHS_RIGHT_DOWN(X) | GLYPHS_BOTTOM(X)
/*
  |_|
    | */
#define GLYPHS_CHAR_4(X)    GLYPHS_LEFT_UP(X) | GLYPHS_MIDDLE(X) | GLYPHS_RIGHT_UP(X) | GLYPHS_RIGHT_DOWN(X)
/* _
  |_
   _|*/
#define GLYPHS_CHAR_5(X)    GLYPHS_TOP(X) | GLYPHS_LEFT_UP(X) | GLYPHS_MIDDLE(X) | GLYPHS_RIGHT_DOWN(X) | GLYPHS_BOTTOM(X)
/* _
  |_
  |_| */
#define GLYPHS_CHAR_6(X)    GLYPHS_TOP(X) | GLYPHS_LEFT_UP(X) | GLYPHS_LEFT_DOWN(X) |GLYPHS_BOTTOM(X) | GLYPHS_RIGHT_DOWN(X) | GLYPHS_MIDDLE(X)
/* _
    |
    | */
#define GLYPHS_CHAR_7(X)    GLYPHS_TOP(X) | GLYPHS_RIGHT_UP(X) | GLYPHS_RIGHT_DOWN(X)
/* _
  |_|
  |_| */
#define GLYPHS_CHAR_8(X)    GLYPHS_TOP(X) | GLYPHS_LEFT_UP(X) | GLYPHS_MIDDLE(X) | GLYPHS_RIGHT_DOWN(X) | GLYPHS_BOTTOM(X) | GLYPHS_LEFT_DOWN(X) | GLYPHS_RIGHT_UP(X)
/* _
  |_|
   _| */
#define GLYPHS_CHAR_9(X)    GLYPHS_TOP(X) | GLYPHS_LEFT_UP(X) | GLYPHS_MIDDLE(X) | GLYPHS_RIGHT_UP(X) | GLYPHS_RIGHT_DOWN(X) | GLYPHS_BOTTOM(X)
#endif
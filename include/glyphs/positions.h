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
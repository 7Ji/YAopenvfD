#ifndef _HAVE_GLYPHS_H
#define _HAVE_GLYPHS_H
#include "common.h"

#define GLYPHS_BOTTOM(X)    GLYPHS_ ## X ## _A
#define GLYPHS_LEFT_DOWN(X) GLYPHS_ ## X ## _B
#define GLYPHS_LEFT_UP(X)   GLYPHS_ ## X ## _C
#define GLYPHS_TOP(X)       GLYPHS_ ## X ## _D
#define GLYPHS_RIGHT_UP(X)  GLYPHS_ ## X ## _E
#define GLYPHS_RIGHT_DOWN(X)GLYPHS_ ## X ## _F
#define GLYPHS_MIDDLE(X)    GLYPHS_ ## X ## _G
#define GLYPHS_POINT(X)     GLYPHS_ ## X ## _P

extern const uint8_t glyphs_lookup_table[][256];
// uint8_t glyphs_lookup_char(char const character, unsigned short table_id);

#endif

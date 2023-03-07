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
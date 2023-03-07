#ifndef _HAVE_GLYPHS_SETUP_H
#define _HAVE_GLYPHS_SETUP_H
#include "glyphs/char.h"

#define GLYPHS_SETUP(X) \
    { \
        /* ASCII 0-9 */     0, 0, 0, 0, 0, 0, 0, 0, 0, 0, \
        /* ASCII 10-19 */   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, \
        /* ASCII 20-29 */   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, \
        /* ASCII 30-36 */   0, 0, 0, 0, 0, 0, 0, \
        /* ASCII 37: % */   GLYPHS_CHAR_PERCENT(X), \
        /* ASCII 38-39 */   0, 0, \
        /* ASCII 40-44 */   0, 0, 0, 0, 0, \
        /* ASCII 45: - */   GLYPHS_CHAR_HYPHEN_MINUS(X), \
        /* ASCII 46 */      0, \
        /* ASCII 47: / */   GLYPHS_CHAR_SLASH(X), \
        /* ASCII 48: 0 */   GLYPHS_CHAR_0(X), \
        /* ASCII 49: 1 */   GLYPHS_CHAR_1(X), \
        /* ASCII 50: 2 */   GLYPHS_CHAR_2(X), \
        /* ASCII 51: 3 */   GLYPHS_CHAR_3(X), \
        /* ASCII 52: 4 */   GLYPHS_CHAR_4(X), \
        /* ASCII 53: 5 */   GLYPHS_CHAR_5(X), \
        /* ASCII 54: 6 */   GLYPHS_CHAR_6(X), \
        /* ASCII 55: 7 */   GLYPHS_CHAR_7(X), \
        /* ASCII 56: 8 */   GLYPHS_CHAR_8(X), \
        /* ASCII 57: 9 */   GLYPHS_CHAR_9(X), \
        /* ASCII 58-59 */   0, 0, \
        /* ASCII 60-64 */   0, 0, 0, 0, 0, \
        /* ASCII 65: A */   GLYPHS_CHAR_UPPER_A(X), \
        /* ASCII 66: B */   GLYPHS_CHAR_UPPER_B(X), \
        /* ASCII 67: C */   GLYPHS_CHAR_UPPER_C(X), \
        /* ASCII 68: D */   GLYPHS_CHAR_UPPER_D(X), \
        /* ASCII 69: E */   GLYPHS_CHAR_UPPER_E(X), \
        /* ASCII 70: F */   GLYPHS_CHAR_UPPER_F(X), \
        /* ASCII 71: G */   GLYPHS_CHAR_UPPER_G(X), \
        /* ASCII 72: H */   GLYPHS_CHAR_UPPER_H(X), \
        /* ASCII 73: I */   GLYPHS_CHAR_UPPER_I(X), \
        /* ASCII 74: J */   GLYPHS_CHAR_UPPER_J(X), \
        /* ASCII 75: K */   GLYPHS_CHAR_UPPER_K(X), \
        /* ASCII 76: L */   GLYPHS_CHAR_UPPER_L(X), \
        /* ASCII 77: M */   GLYPHS_CHAR_UPPER_M(X), \
        /* ASCII 78: N */   GLYPHS_CHAR_UPPER_N(X), \
        /* ASCII 79: O */   GLYPHS_CHAR_UPPER_O(X), \
        /* ASCII 80: P */   GLYPHS_CHAR_UPPER_P(X), \
        /* ASCII 81: Q */   GLYPHS_CHAR_UPPER_Q(X), \
        /* ASCII 82: R */   GLYPHS_CHAR_UPPER_R(X), \
        /* ASCII 83: S */   GLYPHS_CHAR_UPPER_S(X), \
        /* ASCII 84: T */   GLYPHS_CHAR_UPPER_T(X), \
        /* ASCII 85: U */   GLYPHS_CHAR_UPPER_U(X), \
        /* ASCII 86: V */   GLYPHS_CHAR_UPPER_V(X), \
        /* ASCII 87: W */   GLYPHS_CHAR_UPPER_W(X), \
        /* ASCII 88: X */   GLYPHS_CHAR_UPPER_X(X), \
        /* ASCII 89: Y */   GLYPHS_CHAR_UPPER_Y(X), \
        /* ASCII 90: Z */   GLYPHS_CHAR_UPPER_Z(X), \
        /* ASCII 91 */      0, \
        /* ASCII 92: \ */   GLYPHS_CHAR_BACKSLASH(X), \
        /* ASCII 93-94 */   0, 0, \
        /* ASCII 95: _ */   GLYPHS_CHAR_LOW_LINE(X), \
        /* ASCII 96    */   0, \
        /* ASCII 97: a */   GLYPHS_CHAR_LOWER_A(X), \
        /* ASCII 98: b */   GLYPHS_CHAR_LOWER_B(X), \
        /* ASCII 99: c */   GLYPHS_CHAR_LOWER_C(X), \
        /* ASCII 100: d */  GLYPHS_CHAR_LOWER_D(X), \
        /* ASCII 101: e */  GLYPHS_CHAR_LOWER_E(X), \
        /* ASCII 102: f */  GLYPHS_CHAR_LOWER_F(X), \
        /* ASCII 103: g */  GLYPHS_CHAR_LOWER_G(X), \
        /* ASCII 104: h */  GLYPHS_CHAR_LOWER_H(X), \
        /* ASCII 105: i */  GLYPHS_CHAR_LOWER_I(X), \
        /* ASCII 106: j */  GLYPHS_CHAR_LOWER_J(X), \
        /* ASCII 107: k */  GLYPHS_CHAR_LOWER_K(X), \
        /* ASCII 108: l */  GLYPHS_CHAR_LOWER_L(X), \
        /* ASCII 109: m */  GLYPHS_CHAR_LOWER_M(X), \
        /* ASCII 110: n */  GLYPHS_CHAR_LOWER_N(X), \
        /* ASCII 111: o */  GLYPHS_CHAR_LOWER_O(X), \
        /* ASCII 112: p */  GLYPHS_CHAR_LOWER_P(X), \
        /* ASCII 113: q */  GLYPHS_CHAR_LOWER_Q(X), \
        /* ASCII 114: r */  GLYPHS_CHAR_LOWER_R(X), \
        /* ASCII 115: s */  GLYPHS_CHAR_LOWER_S(X), \
        /* ASCII 116: t */  GLYPHS_CHAR_LOWER_T(X), \
        /* ASCII 117: u */  GLYPHS_CHAR_LOWER_U(X), \
        /* ASCII 118: v */  GLYPHS_CHAR_LOWER_V(X), \
        /* ASCII 119: w */  GLYPHS_CHAR_LOWER_W(X), \
        /* ASCII 120: x */  GLYPHS_CHAR_LOWER_X(X), \
        /* ASCII 121: y */  GLYPHS_CHAR_LOWER_Y(X), \
        /* ASCII 122: z */  GLYPHS_CHAR_LOWER_Z(X), \
        /* ASCII 123-129 */ 0, 0, 0, 0, 0, 0, 0, \
        /* ASCII 130-139 */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, \
        /* ASCII 140-149 */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, \
        /* ASCII 150-159 */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, \
        /* ASCII 160-169 */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, \
        /* ASCII 170-175 */ 0, 0, 0, 0, 0, 0, \
        /* ASCII 176: ° */  GLYPHS_CHAR_DEGREE(X), \
        /* ASCII 177-179 */ 0, 0, 0, \
        /* ASCII 180-189 */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, \
        /* ASCII 190-199 */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, \
        /* ASCII 200-209 */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, \
        /* ASCII 210-219 */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, \
        /* ASCII 220-229 */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, \
        /* ASCII 230-239 */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, \
        /* ASCII 240-249 */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, \
        /* ASCII 250-255 */ 0, 0, 0, 0, 0, 0 \
    }
#endif
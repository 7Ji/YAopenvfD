#include "glyphs.h"


#define GLYPHS_BIT(x) 0b00000001 << ( x - 1 )


/*
 * Most displays have 5 sections, 1 - 4 are the digits,
 * the 5th is mapped to indicators (5A-5G).
 * The 7 segment sequences are shown below.
 */

/*=============================*/
/* dp
 *       +--d/08--+
 *       |        |
 *  c/04 |        | e/10
 *       +--g/40--+
 *       |        |
 *  b/02 |        | f/20
 *       +--a/01--+      
 */
#define GLYPHS_0_A  GLYPHS_BIT(1)
#define GLYPHS_0_B  GLYPHS_BIT(2)
#define GLYPHS_0_C  GLYPHS_BIT(3)
#define GLYPHS_0_D  GLYPHS_BIT(4)
#define GLYPHS_0_E  GLYPHS_BIT(5)
#define GLYPHS_0_F  GLYPHS_BIT(6)
#define GLYPHS_0_G  GLYPHS_BIT(7)
#define GLYPHS_0_P  GLYPHS_BIT(8)
/*=============================*/
/* dp
 *       +--d/01--+
 *       |        |
 *  c/20 |        | e/02
 *       +--g/40--+
 *       |        |
 *  b/10 |        | f/04
 *       +--a/08--+      
 */
#define GLYPHS_1_A  GLYPHS_BIT(4)
#define GLYPHS_1_B  GLYPHS_BIT(5)
#define GLYPHS_1_C  GLYPHS_BIT(6)
#define GLYPHS_1_D  GLYPHS_BIT(1)
#define GLYPHS_1_E  GLYPHS_BIT(2)
#define GLYPHS_1_F  GLYPHS_BIT(3)
#define GLYPHS_1_G  GLYPHS_BIT(7)
#define GLYPHS_1_P  GLYPHS_BIT(8)
/*=============================*/
/* dp
 *       +--d/02--+
 *       |        |
 *  c/20 |        | e/10
 *       +--g/40--+
 *       |        |
 *  b/08 |        | f/04
 *       +--a/01--+
 */
#define GLYPHS_2_A  GLYPHS_BIT(1)
#define GLYPHS_2_B  GLYPHS_BIT(4)
#define GLYPHS_2_C  GLYPHS_BIT(6)
#define GLYPHS_2_D  GLYPHS_BIT(2)
#define GLYPHS_2_E  GLYPHS_BIT(5)
#define GLYPHS_2_F  GLYPHS_BIT(3)
#define GLYPHS_2_G  GLYPHS_BIT(7)
#define GLYPHS_2_P  GLYPHS_BIT(8)
/*=============================*/
/*  dp
 *       +--d/01--+
 *       |        |
 *  c/80 |        | e/40
 *       +--g/20--+
 *       |        |
 *  b/02 |        | f/10
 *       +--a/04--+.p/08
*/
#define GLYPHS_3_A  GLYPHS_BIT(3)
#define GLYPHS_3_B  GLYPHS_BIT(2)
#define GLYPHS_3_C  GLYPHS_BIT(8)
#define GLYPHS_3_D  GLYPHS_BIT(1)
#define GLYPHS_3_E  GLYPHS_BIT(7)
#define GLYPHS_3_F  GLYPHS_BIT(5)
#define GLYPHS_3_G  GLYPHS_BIT(6)
#define GLYPHS_3_P  GLYPHS_BIT(4)
/*=============================*/
/*  dp
 *       +--d/01--+
 *       |        |
 *  c/10 |        | e/02
 *       +--g/08--+
 *       |        |
 *  b/20 |        | f/04
 *       +--a/40--+
 */
#define GLYPHS_4_A  GLYPHS_BIT(7)
#define GLYPHS_4_B  GLYPHS_BIT(6)
#define GLYPHS_4_C  GLYPHS_BIT(5)
#define GLYPHS_4_D  GLYPHS_BIT(1)
#define GLYPHS_4_E  GLYPHS_BIT(2)
#define GLYPHS_4_F  GLYPHS_BIT(3)
#define GLYPHS_4_G  GLYPHS_BIT(4)
#define GLYPHS_4_P  GLYPHS_BIT(8)
/*=============================*/

#define GLYPHS_CHAR_0(X)    GLYPHS_BOTTOM(X) | GLYPHS_LEFT_DOWN(X) | GLYPHS_LEFT_UP(X) | GLYPHS_TOP(X) | GLYPHS_RIGHT_UP(X) | GLYPHS_RIGHT_DOWN(X)
#define GLYPHS_CHAR_1(X)    GLYPHS_RIGHT_UP(X) | GLYPHS_RIGHT_DOWN(X)
#define GLYPHS_CHAR_2(X)    GLYPHS_BOTTOM(X) | GLYPHS_LEFT_DOWN(X) | GLYPHS_TOP(X) | GLYPHS_RIGHT_UP(X) | GLYPHS_MIDDLE(X)
#define GLYPHS_CHAR_3(X)    GLYPHS_BOTTOM(X) | GLYPHS_TOP(X) | GLYPHS_RIGHT_UP(X) | GLYPHS_RIGHT_DOWN(X) | GLYPHS_MIDDLE(X)
#define GLYPHS_CHAR_4(X)    GLYPHS_LEFT_UP(X) | GLYPHS_RIGHT_UP(X) | GLYPHS_RIGHT_DOWN(X) | GLYPHS_MIDDLE(X)
#define GLYPHS_CHAR_5(X)    GLYPHS_BOTTOM(X) | GLYPHS_LEFT_UP(X) | GLYPHS_TOP(X) | GLYPHS_RIGHT_DOWN(X) | GLYPHS_MIDDLE(X)
#define GLYPHS_CHAR_6(X)    GLYPHS_BOTTOM(X) | GLYPHS_LEFT_DOWN(X) | GLYPHS_LEFT_UP(X) | GLYPHS_TOP(X) | GLYPHS_RIGHT_DOWN(X) | GLYPHS_MIDDLE(X)
#define GLYPHS_CHAR_7(X)    GLYPHS_TOP(X) | GLYPHS_RIGHT_UP(X) | GLYPHS_RIGHT_DOWN(X)
#define GLYPHS_CHAR_8(X)    GLYPHS_BOTTOM(X) | GLYPHS_LEFT_DOWN(X) | GLYPHS_LEFT_UP(X) | GLYPHS_TOP(X) | GLYPHS_RIGHT_UP(X) | GLYPHS_RIGHT_DOWN(X) | GLYPHS_MIDDLE(X)
#define GLYPHS_CHAR_9(X)    GLYPHS_BOTTOM(X) | GLYPHS_LEFT_UP(X) | GLYPHS_TOP(X) | GLYPHS_RIGHT_UP(X) | GLYPHS_RIGHT_DOWN(X) | GLYPHS_MIDDLE(X)
#define GLYPHS_CHAR_A(X)    GLYPHS_LEFT_DOWN(X) | GLYPHS_LEFT_UP(X) | GLYPHS_TOP(X) | GLYPHS_RIGHT_UP(X) | GLYPHS_RIGHT_DOWN(X) | GLYPHS_MIDDLE(X)
#define GLYPHS_CHAR_B(X)    GLYPHS_BOTTOM(X) | GLYPHS_LEFT_DOWN(X) | GLYPHS_LEFT_UP(X) | GLYPHS_RIGHT_DOWN(X) | GLYPHS_MIDDLE(X)
#define GLYPHS_CHAR_C(X)    GLYPHS_BOTTOM(X) | GLYPHS_LEFT_DOWN(X) | GLYPHS_LEFT_UP(X) | GLYPHS_TOP(X)
#define GLYPHS_CHAR_D(X)    GLYPHS_BOTTOM(X) | GLYPHS_LEFT_DOWN(X) | GLYPHS_RIGHT_UP(X) | GLYPHS_RIGHT_DOWN(X) | GLYPHS_MIDDLE(X)
#define GLYPHS_CHAR_E(X)    GLYPHS_BOTTOM(X) | GLYPHS_LEFT_DOWN(X) | GLYPHS_LEFT_UP(X) | GLYPHS_TOP(X) | GLYPHS_MIDDLE(X)
#define GLYPHS_CHAR_F(X)    GLYPHS_LEFT_DOWN(X) | GLYPHS_LEFT_UP(X) | GLYPHS_TOP(X) | GLYPHS_MIDDLE(X)
#define GLYPHS_CHAR_G(X)    GLYPHS_BOTTOM(X) | GLYPHS_LEFT_DOWN(X) | GLYPHS_LEFT_UP(X) | GLYPHS_TOP(X) | GLYPHS_RIGHT_DOWN(X)
#define GLYPHS_CHAR_H(X)    GLYPHS_LEFT_DOWN(X) | GLYPHS_LEFT_UP(X) | GLYPHS_RIGHT_DOWN(X) | GLYPHS_MIDDLE(X)
#define GLYPHS_CHAR_I(X)    GLYPHS_LEFT_DOWN(X) | GLYPHS_LEFT_UP(X)
#define GLYPHS_CHAR_J(X)    GLYPHS_BOTTOM(X) | GLYPHS_LEFT_DOWN(X) | GLYPHS_RIGHT_UP(X) | GLYPHS_RIGHT_DOWN(X)
#define GLYPHS_CHAR_K(X)    GLYPHS_LEFT_DOWN(X) | GLYPHS_LEFT_UP(X) | GLYPHS_TOP(X) | GLYPHS_RIGHT_DOWN(X) | GLYPHS_MIDDLE(X)
#define GLYPHS_CHAR_L(X)    GLYPHS_BOTTOM(X) | GLYPHS_LEFT_DOWN(X) | GLYPHS_LEFT_UP(X)
#define GLYPHS_CHAR_M(X)    GLYPHS_LEFT_DOWN(X) | GLYPHS_TOP(X) | GLYPHS_RIGHT_DOWN(X)
#define GLYPHS_CHAR_N(X)    GLYPHS_LEFT_DOWN(X) | GLYPHS_LEFT_UP(X) | GLYPHS_TOP(X) | GLYPHS_RIGHT_UP(X) | GLYPHS_RIGHT_DOWN(X)
#define GLYPHS_CHAR_O(X)    GLYPHS_BOTTOM(X) | GLYPHS_LEFT_DOWN(X) | GLYPHS_RIGHT_DOWN(X) | GLYPHS_MIDDLE(X)
#define GLYPHS_CHAR_P(X)    GLYPHS_LEFT_DOWN(X) | GLYPHS_LEFT_UP(X) | GLYPHS_TOP(X) | GLYPHS_RIGHT_UP(X) | GLYPHS_MIDDLE(X)
#define GLYPHS_CHAR_Q(X)    GLYPHS_LEFT_UP(X) | GLYPHS_TOP(X) | GLYPHS_RIGHT_UP(X) | GLYPHS_RIGHT_DOWN(X) | GLYPHS_MIDDLE(X)
#define GLYPHS_CHAR_R(X)    GLYPHS_LEFT_DOWN(X) | GLYPHS_MIDDLE(X)
#define GLYPHS_CHAR_S(X)    GLYPHS_BOTTOM(X) | GLYPHS_LEFT_UP(X) | GLYPHS_TOP(X) | GLYPHS_RIGHT_DOWN(X) | GLYPHS_MIDDLE(X)
#define GLYPHS_CHAR_T(X)    GLYPHS_BOTTOM(X) | GLYPHS_LEFT_DOWN(X) | GLYPHS_LEFT_UP(X) | GLYPHS_MIDDLE(X)
#define GLYPHS_CHAR_U(X)    GLYPHS_BOTTOM(X) | GLYPHS_LEFT_DOWN(X) | GLYPHS_RIGHT_DOWN(X)
#define GLYPHS_CHAR_V(X)    GLYPHS_BOTTOM(X) | GLYPHS_LEFT_DOWN(X) | GLYPHS_LEFT_UP(X) | GLYPHS_RIGHT_UP(X) | GLYPHS_RIGHT_DOWN(X)
#define GLYPHS_CHAR_W(X)    GLYPHS_BOTTOM(X) | GLYPHS_LEFT_UP(X) | GLYPHS_RIGHT_UP(X)
#define GLYPHS_CHAR_X(X)    GLYPHS_LEFT_DOWN(X) | GLYPHS_LEFT_UP(X) | GLYPHS_RIGHT_UP(X) | GLYPHS_RIGHT_DOWN(X) | GLYPHS_MIDDLE(X)
#define GLYPHS_CHAR_Y(X)    GLYPHS_BOTTOM(X) | GLYPHS_LEFT_UP(X) | GLYPHS_RIGHT_UP(X) | GLYPHS_RIGHT_DOWN(X) | GLYPHS_MIDDLE(X)
#define GLYPHS_CHAR_Z(X)    GLYPHS_BOTTOM(X) | GLYPHS_LEFT_DOWN(X) | GLYPHS_TOP(X) | GLYPHS_RIGHT_UP(X) | GLYPHS_MIDDLE(X)
#define GLYPHS_CHAR_HYPHEN_MINUS(X) \
                            GLYPHS_MIDDLE(X)
#define GLYPHS_CHAR_LOW_LINE(X) \
                            GLYPHS_BOTTOM(X)
#define GLYPHS_CHAR_DEGREE(X) \
                            GLYPHS_LEFT_UP(X) | GLYPHS_TOP(X) | GLYPHS_RIGHT_UP(X) | GLYPHS_MIDDLE(X)


#define GLYPHS_SETUP(X) \
    { \
        /* ASCII 0-9 */     0, 0, 0, 0, 0, 0, 0, 0, 0, 0, \
        /* ASCII 10-19 */   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, \
        /* ASCII 20-29 */   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, \
        /* ASCII 30-39 */   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, \
        /* ASCII 40-44 */   0, 0, 0, 0, 0, \
        /* ASCII 45: - */   GLYPHS_CHAR_HYPHEN_MINUS(X), \
        /* ASCII 46-47 */   0, 0, \
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
        /* ASCII 65: A */   GLYPHS_CHAR_A(X), \
        /* ASCII 66: B */   GLYPHS_CHAR_B(X), \
        /* ASCII 67: C */   GLYPHS_CHAR_C(X), \
        /* ASCII 68: D */   GLYPHS_CHAR_D(X), \
        /* ASCII 69: E */   GLYPHS_CHAR_E(X), \
        /* ASCII 70: F */   GLYPHS_CHAR_F(X), \
        /* ASCII 71: G */   GLYPHS_CHAR_G(X), \
        /* ASCII 72: H */   GLYPHS_CHAR_H(X), \
        /* ASCII 73: I */   GLYPHS_CHAR_I(X), \
        /* ASCII 74: J */   GLYPHS_CHAR_J(X), \
        /* ASCII 75: K */   GLYPHS_CHAR_K(X), \
        /* ASCII 76: L */   GLYPHS_CHAR_L(X), \
        /* ASCII 77: M */   GLYPHS_CHAR_M(X), \
        /* ASCII 78: N */   GLYPHS_CHAR_N(X), \
        /* ASCII 79: O */   GLYPHS_CHAR_O(X), \
        /* ASCII 80: P */   GLYPHS_CHAR_P(X), \
        /* ASCII 81: Q */   GLYPHS_CHAR_Q(X), \
        /* ASCII 82: R */   GLYPHS_CHAR_R(X), \
        /* ASCII 83: S */   GLYPHS_CHAR_S(X), \
        /* ASCII 84: T */   GLYPHS_CHAR_T(X), \
        /* ASCII 85: U */   GLYPHS_CHAR_U(X), \
        /* ASCII 86: V */   GLYPHS_CHAR_V(X), \
        /* ASCII 87: W */   GLYPHS_CHAR_W(X), \
        /* ASCII 88: X */   GLYPHS_CHAR_X(X), \
        /* ASCII 89: Y */   GLYPHS_CHAR_Y(X), \
        /* ASCII 90: Z */   GLYPHS_CHAR_Z(X), \
        /* ASCII 91-96 */   0, 0, 0, 0, 0, 0, \
        /* ASCII 97: a */   GLYPHS_CHAR_A(X), \
        /* ASCII 98: b */   GLYPHS_CHAR_B(X), \
        /* ASCII 99: c */   GLYPHS_CHAR_C(X), \
        /* ASCII 100: d */  GLYPHS_CHAR_D(X), \
        /* ASCII 101: e */  GLYPHS_CHAR_E(X), \
        /* ASCII 102: f */  GLYPHS_CHAR_F(X), \
        /* ASCII 103: g */  GLYPHS_CHAR_G(X), \
        /* ASCII 104: h */  GLYPHS_CHAR_H(X), \
        /* ASCII 105: i */  GLYPHS_CHAR_I(X), \
        /* ASCII 106: j */  GLYPHS_CHAR_J(X), \
        /* ASCII 107: k */  GLYPHS_CHAR_K(X), \
        /* ASCII 108: l */  GLYPHS_CHAR_L(X), \
        /* ASCII 109: m */  GLYPHS_CHAR_M(X), \
        /* ASCII 110: n */  GLYPHS_CHAR_N(X), \
        /* ASCII 111: o */  GLYPHS_CHAR_O(X), \
        /* ASCII 112: p */  GLYPHS_CHAR_P(X), \
        /* ASCII 113: q */  GLYPHS_CHAR_Q(X), \
        /* ASCII 114: r */  GLYPHS_CHAR_R(X), \
        /* ASCII 115: s */  GLYPHS_CHAR_S(X), \
        /* ASCII 116: t */  GLYPHS_CHAR_T(X), \
        /* ASCII 117: u */  GLYPHS_CHAR_U(X), \
        /* ASCII 118: v */  GLYPHS_CHAR_V(X), \
        /* ASCII 119: w */  GLYPHS_CHAR_W(X), \
        /* ASCII 120: x */  GLYPHS_CHAR_X(X), \
        /* ASCII 121: y */  GLYPHS_CHAR_Y(X), \
        /* ASCII 122: z */  GLYPHS_CHAR_Z(X), \
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

static const uint8_t glyphs_ascii_lookup_table[][256] = {
    GLYPHS_SETUP(0),
    GLYPHS_SETUP(1),
    GLYPHS_SETUP(2),
    GLYPHS_SETUP(3),
    GLYPHS_SETUP(4)
};

// void glyphs_debugging() {
//     // for (unsigned short i = 0; i < 5; ++i) {
//         uint8_t const *const table = glyphs_ascii_lookup_table[0];
//         for (uint8_t c = 0;; ++c) {
//             uint8_t glyph = table[c];
//             printf("character %hu (%c):\n", c, c);
//             if (glyph & GLYPHS_TOP(0)) {
//                 printf("   -  \n");
//             } else {
//                 printf("\n");
//             }
//             if (glyph & GLYPHS_LEFT_UP(0)) {
//                 printf(" |   ");
//             } else {
//                 printf("     ");
//             }
//             if (glyph & GLYPHS_RIGHT_UP(0)) {
//                 printf("|\n");
//             } else {
//                 printf("\n");
//             }
//             if (glyph & GLYPHS_MIDDLE(0)) {
//                 printf("   -\n");
//             } else {
//                 printf("\n");
//             }
//             if (glyph & GLYPHS_LEFT_DOWN(0)) {
//                 printf(" |   ");
//             } else {
//                 printf("     ");
//             }
//             if (glyph & GLYPHS_RIGHT_DOWN(0)) {
//                 printf("|\n");
//             } else {
//                 printf("\n");
//             }
//             if (glyph & GLYPHS_BOTTOM(0)) {
//                 printf("   -  ");
//             } else {
//                 printf("      ");
//             }
//             if (glyph & GLYPHS_POINT(0)) {
//                 printf(".\n");
//             } else {
//                 printf("\n");
//             }
//             if (c == 0xffU) {
//                 break;
//             }
//         }
//     // }
// }
/*
YAopenvfD/include/glyphs/displays.h: display-dependant glyph stroke data for 7-segment LED display
Copyright (C) 2023 Guoxin "7Ji" Pu <pugokushin@gmail.com>

This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with this program. If not, see <https://www.gnu.org/licenses/>.
*/

#ifndef _HAVE_GLYPHS_DISPLAYS_H
#define _HAVE_GLYPHS_DISPLAYS_H
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
#define GLYPHS_0_A  0b00000001
#define GLYPHS_0_B  0b00000010
#define GLYPHS_0_C  0b00000100
#define GLYPHS_0_D  0b00001000
#define GLYPHS_0_E  0b00010000
#define GLYPHS_0_F  0b00100000
#define GLYPHS_0_G  0b01000000
#define GLYPHS_0_P  0b10000000
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
#define GLYPHS_1_A  0b00001000
#define GLYPHS_1_B  0b00010000
#define GLYPHS_1_C  0b00100000
#define GLYPHS_1_D  0b00000001
#define GLYPHS_1_E  0b00000010
#define GLYPHS_1_F  0b00000100
#define GLYPHS_1_G  0b01000000
#define GLYPHS_1_P  0b10000000
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
#define GLYPHS_2_A  0b00000001
#define GLYPHS_2_B  0b00001000
#define GLYPHS_2_C  0b00100000
#define GLYPHS_2_D  0b00000010
#define GLYPHS_2_E  0b00010000
#define GLYPHS_2_F  0b00000100
#define GLYPHS_2_G  0b01000000
#define GLYPHS_2_P  0b10000000
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
#define GLYPHS_3_A  0b00000100
#define GLYPHS_3_B  0b00000010
#define GLYPHS_3_C  0b10000000
#define GLYPHS_3_D  0b00000001
#define GLYPHS_3_E  0b01000000
#define GLYPHS_3_F  0b00010000
#define GLYPHS_3_G  0b00100000
#define GLYPHS_3_P  0b00001000
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
#define GLYPHS_4_A  0b01000000
#define GLYPHS_4_B  0b00100000
#define GLYPHS_4_C  0b00010000
#define GLYPHS_4_D  0b00000001
#define GLYPHS_4_E  0b00000010
#define GLYPHS_4_F  0b00000100
#define GLYPHS_4_G  0b00001000
#define GLYPHS_4_P  0b10000000
/*=============================*/
#endif
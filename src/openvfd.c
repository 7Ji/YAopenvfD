#include "openvfd.h"
#include "glyphs.h"
#include "dots.h"
#include <fcntl.h>
#define OPENVFD_NAME    "openvfd"
#define OPENVFD_DEV     "/dev/" OPENVFD_NAME

static int openvfd_fd = -1;
static unsigned short openvfd_glyphs_lookup_id = 0;
static unsigned short const openvfd_dots_lookup_id = 0;
static bool openvfd_char_no_lookup = false;

struct openvfd_write_sequence {
    uint8_t dots;
    uint8_t pad_0;
    uint8_t char_1;
    uint8_t pad_1;
    uint8_t char_2;
    uint8_t pad_2;
    uint8_t char_3;
    uint8_t pad_3;
    uint8_t char_4;
    uint8_t pad_4;
};

static inline void openvfd_clean_write_sequance(struct openvfd_write_sequence *const sequence) {
    sequence->char_1 = '\0';
    sequence->char_2 = '\0';
    sequence->char_3 = '\0';
    sequence->char_4 = '\0';
}

int openvfd_prepare() {
    if ((openvfd_fd = open(OPENVFD_DEV, O_RDWR)) < 0) {
        pr_error_with_errno("Failed to open '"OPENVFD_DEV"' for I/O operation");
        return 1;
    }
    struct openvfd_display display = {0};
    if (ioctl(openvfd_fd, OPENVFD_IOC_GDISPLAY_TYPE, &display)) {
        pr_error_with_errno("Failed to read display type");
        return 2;
    } 
    switch(display.type) {
        case OPENVFD_DISPLAY_TYPE_5D_7S_T95:
            openvfd_glyphs_lookup_id = 0;
            break;
        case OPENVFD_DISPLAY_TYPE_5D_7S_G9SX:
            openvfd_glyphs_lookup_id = 2;
            break;
        case OPENVFD_DISPLAY_TYPE_4D_7S_FREESATGTC:
            openvfd_glyphs_lookup_id = 3;
            break;
        case OPENVFD_DISPLAY_TYPE_5D_7S_TAP1:
            openvfd_glyphs_lookup_id = 4;
            break;
        default:
            openvfd_glyphs_lookup_id = 1;
            break;
    }
    pr_debug("Using glyphs lookup id %d\n", openvfd_glyphs_lookup_id);
    if (display.controller > OPENVFD_CONTROLLER_7S_MAX) {
        openvfd_char_no_lookup = true;
    }
    return 0;
}

void openvfd_write_report(char const report[5], bool const blink) {
    static struct openvfd_write_sequence sequence = {0};
// #ifdef DEBUGGING
//     sequence.dots = 
//         dots_lookup_table[openvfd_dots_lookup_id][DOTS_TYPE_ALARM] |
//         dots_lookup_table[openvfd_dots_lookup_id][DOTS_TYPE_USB] |
//         dots_lookup_table[openvfd_dots_lookup_id][DOTS_TYPE_PLAY] |
//         dots_lookup_table[openvfd_dots_lookup_id][DOTS_TYPE_PAUSE] |
//         dots_lookup_table[openvfd_dots_lookup_id][DOTS_TYPE_SEC] |dots_lookup_table[openvfd_dots_lookup_id][DOTS_TYPE_ETH] |
//         dots_lookup_table[openvfd_dots_lookup_id][DOTS_TYPE_WIFI];
// #endif
    if (blink) {
        static bool colon = true;
        if (colon) {
            sequence.dots = dots_lookup_table[openvfd_dots_lookup_id][DOTS_TYPE_SEC];
            colon = false;
        } else {
            sequence.dots = 0;
            colon = true;
        }
    } else {
        sequence.dots = 0;
    }
    if (openvfd_char_no_lookup) {
        sequence.char_1 = report[0];
        sequence.char_2 = report[1];
        sequence.char_3 = report[2];
        sequence.char_4 = report[3];
    } else {
        sequence.char_1 = glyphs_lookup_table[openvfd_glyphs_lookup_id][(uint8_t)report[0]];
        sequence.char_2 = glyphs_lookup_table[openvfd_glyphs_lookup_id][(uint8_t)report[1]];
        sequence.char_3 = glyphs_lookup_table[openvfd_glyphs_lookup_id][(uint8_t)report[2]];
        sequence.char_4 = glyphs_lookup_table[openvfd_glyphs_lookup_id][(uint8_t)report[3]];
    }
    pr_debug(
        "Characters:\n"
            "\tdots: 0x%x 0b"FORMAT_BIT_SEQUANCE"\n"
            "\t1: %c 0x%x 0b"FORMAT_BIT_SEQUANCE"\n"
            "\t2: %c 0x%x 0b"FORMAT_BIT_SEQUANCE"\n"
            "\t3: %c 0x%x 0b"FORMAT_BIT_SEQUANCE"\n"
            "\t4: %c 0x%x 0b"FORMAT_BIT_SEQUANCE"\n", 
            sequence.dots, expand_bit_sequance(sequence.dots),
            report[0], report[0], expand_bit_sequance(sequence.char_1),
            report[1], report[1], expand_bit_sequance(sequence.char_2),
            report[2], report[2], expand_bit_sequance(sequence.char_3),
            report[3], report[3], expand_bit_sequance(sequence.char_4));
    write(openvfd_fd, &sequence, sizeof sequence);
}
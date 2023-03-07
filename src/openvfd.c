#include "openvfd.h"
#include "glyphs.h"
#include <sys/ioctl.h>
#include <fcntl.h>
#define OPENVFD_NAME    "openvfd"
#define OPENVFD_DEV     "/dev/" OPENVFD_NAME
#define OPENVFD_IOC_MAGIC           'M'
#define OPENVFD_IOC_GDISPLAY_TYPE   _IOR(OPENVFD_IOC_MAGIC,  8, int)

static int openvfd_fd = -1;
static unsigned short openvfd_glyphs_lookup_id = 0;
static unsigned short openvfd_dots_lookup_id = 0;
static bool openvfd_char_no_lookup = false;
static uint8_t openvfd_colon_add = 0;
static uint8_t openvfd_colon_remove = 0;

enum openvfd_display_mode {
    OPENVFD_DISPLAY_MODE_NONE,
    OPENVFD_DISPLAY_MODE_CLOCK,
    OPENVFD_DISPLAY_MODE_CHANNEL,
    OPENVFD_DISPLAY_MODE_PLAYBACK_TIME,
    OPENVFD_DISPLAY_MODE_TITLE,
    OPENVFD_DISPLAY_MODE_TEMPERATURE,
    OPENVFD_DISPLAY_MODE_DATE,
    OPENVFD_DISPLAY_MODE_MAX,
};

enum openvfd_controller {
    OPENVFD_CONTROLLER_FD628        = 0x00,
    OPENVFD_CONTROLLER_FD620,
    OPENVFD_CONTROLLER_TM1618,
    OPENVFD_CONTROLLER_FD650,
    OPENVFD_CONTROLLER_HBS658,
    OPENVFD_CONTROLLER_FD655,
    OPENVFD_CONTROLLER_FD6551,
    OPENVFD_CONTROLLER_7S_MAX,
    OPENVFD_CONTROLLER_IL3829        = 0xFA,
    OPENVFD_CONTROLLER_PCD8544,
    OPENVFD_CONTROLLER_SH1106,
    OPENVFD_CONTROLLER_SSD1306,
    OPENVFD_CONTROLLER_HD44780,
};

enum openvfd_display_type {
    OPENVFD_DISPLAY_TYPE_5D_7S_NORMAL,        // T95U
    OPENVFD_DISPLAY_TYPE_5D_7S_T95,                // T95K is different.
    OPENVFD_DISPLAY_TYPE_5D_7S_X92,
    OPENVFD_DISPLAY_TYPE_5D_7S_ABOX,
    OPENVFD_DISPLAY_TYPE_FD620_REF,
    OPENVFD_DISPLAY_TYPE_4D_7S_COL,
    OPENVFD_DISPLAY_TYPE_5D_7S_M9_PRO,
    OPENVFD_DISPLAY_TYPE_5D_7S_G9SX,
    OPENVFD_DISPLAY_TYPE_4D_7S_FREESATGTC,
    OPENVFD_DISPLAY_TYPE_5D_7S_TAP1,
    OPENVFD_DISPLAY_TYPE_MAX,
};

struct openvfd_display {
    uint8_t type;
    uint8_t reserved;
    uint8_t flags;
    uint8_t controller;
};

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
    switch(display.type) {
        case OPENVFD_DISPLAY_TYPE_5D_7S_X92:
            openvfd_dots_lookup_id = 1;
            break;
        case OPENVFD_DISPLAY_TYPE_5D_7S_ABOX:
            openvfd_dots_lookup_id = 2;
            break;
        case OPENVFD_DISPLAY_TYPE_5D_7S_M9_PRO:
            openvfd_dots_lookup_id = 3;
            break;
        default:
            openvfd_dots_lookup_id = 0;
            break;
    }
    if (display.controller > OPENVFD_CONTROLLER_7S_MAX) {
        openvfd_char_no_lookup = true;
    }
    openvfd_colon_add = 1 << dots_bit_map_table[dots_lookup_table[openvfd_dots_lookup_id][DOTS_TYPE_SEC]];
    openvfd_colon_remove = ~(1 << dots_bit_map_table[dots_lookup_table[openvfd_dots_lookup_id][DOTS_TYPE_SEC]]);
    pr_debug("Using glyphs lookup id %d, dots lookup %d, char no lookup: %s\n", openvfd_glyphs_lookup_id, openvfd_dots_lookup_id, openvfd_char_no_lookup ? "yes" : "no");
    return 0;
}

uint8_t openvfd_lookup_dots(enum dots_type const type) {
    if (type) {
        return 1 << dots_bit_map_table[dots_lookup_table[openvfd_dots_lookup_id][type]];
    } else {
        return 0;
    }
}

int openvfd_write_report(uint32_t word, uint8_t dots, bool const blink) {
    struct openvfd_write_sequence sequence = {0};
    char report[4];
    *((uint32_t *)report) = word;
    sequence.dots = dots;
    if (blink) {
        static bool colon = true;
        if (colon) {
            sequence.dots |= openvfd_colon_add;
            colon = false;
        } else {
            sequence.dots &= openvfd_colon_remove;
            colon = true;
        }
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
            "\tdots:0x%02x 0b"FORMAT_BIT_SEQUANCE"\n"
            "\t1: %c 0x%02x 0b"FORMAT_BIT_SEQUANCE"\n"
            "\t2: %c 0x%02x 0b"FORMAT_BIT_SEQUANCE"\n"
            "\t3: %c 0x%02x 0b"FORMAT_BIT_SEQUANCE"\n"
            "\t4: %c 0x%02x 0b"FORMAT_BIT_SEQUANCE"\n", 
            sequence.dots, expand_bit_sequance(sequence.dots),
            report[0], report[0], expand_bit_sequance(sequence.char_1),
            report[1], report[1], expand_bit_sequance(sequence.char_2),
            report[2], report[2], expand_bit_sequance(sequence.char_3),
            report[3], report[3], expand_bit_sequance(sequence.char_4));
    if (write(openvfd_fd, &sequence, sizeof sequence) < 0) {
        pr_error_with_errno("Failed to write to openvfd dev");
        return 1;
    }
    return 0;
}
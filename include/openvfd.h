#ifndef _HAVE_OPENVFD_H
#define _HAVE_OPENVFD_H
#include "common.h"
#include "dots.h"
#include <sys/ioctl.h>
/* Use 'M' as magic number */
#define OPENVFD_IOC_MAGIC           'M'
#define OPENVFD_IOC_SMODE           _IOW(OPENVFD_IOC_MAGIC,  1, int)
#define OPENVFD_IOC_GMODE           _IOR(OPENVFD_IOC_MAGIC,  2, int)
#define OPENVFD_IOC_SBRIGHT         _IOW(OPENVFD_IOC_MAGIC,  3, int)
#define OPENVFD_IOC_GBRIGHT         _IOR(OPENVFD_IOC_MAGIC,  4, int)
#define OPENVFD_IOC_POWER           _IOW(OPENVFD_IOC_MAGIC,  5, int)
#define OPENVFD_IOC_GVER            _IOR(OPENVFD_IOC_MAGIC,  6, int)
#define OPENVFD_IOC_STATUS_LED      _IOW(OPENVFD_IOC_MAGIC,  7, int)
#define OPENVFD_IOC_GDISPLAY_TYPE   _IOR(OPENVFD_IOC_MAGIC,  8, int)
#define OPENVFD_IOC_SDISPLAY_TYPE   _IOW(OPENVFD_IOC_MAGIC,  9, int)
#define OPENVFD_IOC_SCHARS_ORDER    _IOW(OPENVFD_IOC_MAGIC, 10, uint8_t[7])
#define OPENVFD_IOC_USE_DTB_CONFIG  _IOW(OPENVFD_IOC_MAGIC, 11, int)
#define OPENVFD_IOC_MAXNR           12

#define OPENVFD_DEV_NAME           "openvfd"

#define OPENVFD_DISPLAY_FLAG_TRANSPOSED     0x01
#define OPENVFD_DISPLAY_FLAG_TRANSPOSED_INT 0x00010000
#define OPENVFD_DISPLAY_FLAG_LOW_FREQ       0x40
#define OPENVFD_DISPLAY_FLAG_LOW_FREQ_INT   0x00400000

#define OPENVFD_LED_DOT_SEC        OPENVFD_LED_DOT1_SEC
#define OPENVFD_LED_DOT_MAX        OPENVFD_LED_DOT1_MAX

#define OPENVFD_FD628_DISP_ON        0x08        /* FD628 Display On */
#define OPENVFD_FD628_DISP_OFF       0x00        /* FD628 Display Off */

struct openvfd_display {
    uint8_t type;
    uint8_t reserved;
    uint8_t flags;
    uint8_t controller;
};

struct openvfd_display_data {
    uint16_t mode;
    uint8_t colon_on;
    uint8_t temperature;

    struct {
            uint8_t seconds;
            uint8_t minutes;
            uint8_t hours;
            uint8_t day_of_week;
            uint8_t day;
            uint8_t month;
            uint16_t year;
    } time_date;
    struct {
            uint8_t seconds;
            uint8_t minutes;
            uint8_t hours;
            uint8_t _reserved;
    } time_secondary;
    struct {
            uint16_t channel;
            uint16_t channel_count;
    } channel_data;

    char string_main[512];
    char string_secondary[128];
};


enum openvfd_protocol {
    OPENVFD_PROTOCOL_NONE,
    OPENVFD_PROTOCOL_I2C,
    OPENVFD_PROTOCOL_MAX
};

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

enum openvfd_fd628_brightness {   /* FD628 Brightness levels */
    OPENVFD_FD628_Brightness_1 = 0x00,
    OPENVFD_FD628_Brightness_2,
    OPENVFD_FD628_Brightness_3,
    OPENVFD_FD628_Brightness_4,
    OPENVFD_FD628_Brightness_5,
    OPENVFD_FD628_Brightness_6,
    OPENVFD_FD628_Brightness_7,
    OPENVFD_FD628_Brightness_8
};

int openvfd_prepare();
uint8_t openvfd_lookup_dots(enum dots_type type);
int openvfd_write_report(uint32_t word, uint8_t dots, bool const blink);

#endif

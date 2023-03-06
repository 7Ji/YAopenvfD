#include "dots.h"

#define DOTS_SETUP(TYPE, BIT) \
    [DOTS_TYPE_ ##TYPE] = 1 << (BIT - 1)

char const dots_type_strings[][10] = {
    "alarm"
    "usb",
    "play",
    "pause",
    "sec",
    "eth",
    "wifi",
    "apps",
    "setup",
    "card",
    "hdmi",
    "cvbs",
    "power",
    "lan",
    "wifihi",
    "wifilo",
    "bt",
    "spdif",
    "av"
};

uint8_t const dots_lookup_table[][DOTS_TYPE_AV+1] = {
    {
        DOTS_SETUP(ALARM, 1),
        DOTS_SETUP(USB, 2),
        DOTS_SETUP(PLAY, 3),
        DOTS_SETUP(PAUSE, 4),
        DOTS_SETUP(SEC, 5),
        DOTS_SETUP(ETH, 6),
        DOTS_SETUP(WIFI, 7)
    },
    {
        DOTS_SETUP(APPS, 1),
        DOTS_SETUP(SETUP, 2),
        DOTS_SETUP(USB, 3),
        DOTS_SETUP(CARD, 4),
        DOTS_SETUP(SEC, 5),
        DOTS_SETUP(HDMI, 6),
        DOTS_SETUP(CVBS, 7)
    },
    {
        DOTS_SETUP(POWER, 3),
        DOTS_SETUP(LAN, 4),
        DOTS_SETUP(SEC, 5),
        DOTS_SETUP(WIFIHI, 6),
        DOTS_SETUP(WIFILO, 7)
    },
    {
        DOTS_SETUP(BT, 1),
        DOTS_SETUP(ETH, 2),
        DOTS_SETUP(WIFI, 3),
        DOTS_SETUP(SPDIF, 4),
        DOTS_SETUP(SEC, 5),
        DOTS_SETUP(HDMI, 6),
        DOTS_SETUP(AV, 7)
    }
};
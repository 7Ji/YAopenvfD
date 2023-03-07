#ifndef _HAVE_DOTS_H
#define _HAVE_DOTS_H
#include "common.h"

enum dots_type {
    DOTS_TYPE_NONE,
    DOTS_TYPE_ALARM,
    DOTS_TYPE_USB,
    DOTS_TYPE_PLAY,
    DOTS_TYPE_PAUSE,
    DOTS_TYPE_SEC,
    DOTS_TYPE_ETH,
    DOTS_TYPE_WIFI,
    DOTS_TYPE_APPS,
    DOTS_TYPE_SETUP,
    DOTS_TYPE_CARD,
    DOTS_TYPE_HDMI,
    DOTS_TYPE_CVBS,
    DOTS_TYPE_POWER,
    DOTS_TYPE_LAN,
    DOTS_TYPE_WIFIHI,
    DOTS_TYPE_WIFILO,
    DOTS_TYPE_BT,
    DOTS_TYPE_SPDIF,
    DOTS_TYPE_AV
};

// extern char const dots_type_strings[][10];
extern uint8_t const dots_lookup_table[][DOTS_TYPE_AV+1];
enum dots_type dots_get_type_from_string(char const *string);
#endif
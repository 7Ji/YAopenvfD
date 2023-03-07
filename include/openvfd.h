#ifndef _HAVE_OPENVFD_H
#define _HAVE_OPENVFD_H
#include "common.h"
#include "dots.h"

int openvfd_prepare();
uint8_t openvfd_lookup_dots(enum dots_type type);
int openvfd_write_report(uint32_t word, uint8_t dots, bool const blink);

#endif

#ifndef _HAVE_COMMON_H
#define _HAVE_COMMON_H
#define _GNU_SOURCE
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <linux/limits.h>

#define _cleanup_(x) __attribute__((__cleanup__(x)))

#define pr_error(format, arg...) \
    fprintf(stderr, "%s:%d: "format, __FUNCTION__, __LINE__, ##arg)

#define pr_error_with_errno(format, arg...) \
    pr_error(format", errno: %d, error: %s\n", ##arg, errno, strerror(errno))

#define pr_warn(format, arg...) \
    printf("%s:%d: "format, __FUNCTION__, __LINE__, ##arg)

#ifdef DEBUGGING
#define pr_debug(format, arg...) \
    printf("%s:%d(debug): "format, __FUNCTION__, __LINE__, ##arg)
#else
#define pr_debug(format, arg...)
#endif

#define FORMAT_BIT_SEQUANCE "%c%c%c%c%c%c%c%c"

#define expand_bit_sequance(x) \
    x & 0b10000000 ? '1' : '0', \
    x & 0b01000000 ? '1' : '0', \
    x & 0b00100000 ? '1' : '0', \
    x & 0b00010000 ? '1' : '0', \
    x & 0b00001000 ? '1' : '0', \
    x & 0b00000100 ? '1' : '0', \
    x & 0b00000010 ? '1' : '0', \
    x & 0b00000001 ? '1' : '0'

static inline void free_p(void *p) {
    if (*(void **)p) free(*(void**) p);
}

static inline void close_p(int *p) {
    if (*p >= 0) close (*p);
}

#endif
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

static inline void free_p(void *p) {
    if (*(void **)p) free(*(void**) p);
}

static inline void close_p(int *p) {
    if (*p >= 0) close (*p);
}

#endif
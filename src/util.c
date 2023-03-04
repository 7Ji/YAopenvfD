#include "util.h"

#define UTIL_SIZE_SUFFIXES   "BKMGTPEZY"
static const unsigned short util_len_size_suffixes = strlen(UTIL_SIZE_SUFFIXES);
static const char util_size_suffixes[] = UTIL_SIZE_SUFFIXES;

size_t util_size_to_human_readable(size_t size, char *suffix) {
    for (unsigned short i = 0; i < util_len_size_suffixes; ++i) {
        if (size < 0x400) {
            *suffix = util_size_suffixes[i];
            return size;
        }
        size /= 0x400;
    }
    *suffix = 'B';
    return size;
}
#include "collector/size.h"

#define COLLECTOR_SIZE_SUFFIXES   "BKMGTPEZY"
static const unsigned short collector_len_size_suffixes = strlen(COLLECTOR_SIZE_SUFFIXES);
static const char collector_size_suffixes[] = COLLECTOR_SIZE_SUFFIXES;

size_t collector_size_to_human_readable(size_t size, char *suffix) {
    for (unsigned short i = 0; i < collector_len_size_suffixes; ++i) {
        if (size < 0x400) {
            *suffix = collector_size_suffixes[i];
            return size;
        }
        size /= 0x400;
    }
    *suffix = 'B';
    return size;
}
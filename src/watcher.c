#include "watcher.h"
#include "openvfd.h"
#include <fcntl.h>
#include <sys/stat.h>

#define WATCHER_TYPE_MAX WATCHER_TYPE_NET_CARRIER
#define WATCHER_PARSE_ARGUMENT_MAX_LEN 128
#define WATCHER_PARSE_ARGUMENT_MAX_SEP 2

static const char watcher_type_strings[][12] = {
    "",
    "file",
    "dev",
    "sys",
    "net_carrier"
};

static const char watcher_file_prefix_table[][6] = {
    [WATCHER_TYPE_DEV] = "/dev/",
    [WATCHER_TYPE_SYS] = "/sys/"
};

char const *watcher_get_type_string(enum watcher_type const type) {
    return watcher_type_strings[type];
}

enum watcher_type watcher_get_type_from_string(char const *const string) {
    if (!string || !string[0]) {
        return WATCHER_TYPE_NONE;
    }
    for (enum watcher_type type = WATCHER_TYPE_NONE + 1; type <= WATCHER_TYPE_MAX; ++type) {
        if (!strcmp(watcher_type_strings[type], string)) {
            return type;
        }
    }
    return WATCHER_TYPE_NONE;
}

static inline size_t watcher_parse_argument_safe_len(size_t len) {
    if (len > WATCHER_PARSE_ARGUMENT_MAX_LEN - 1) {
        return WATCHER_PARSE_ARGUMENT_MAX_LEN - 1;
    } else {
        return len;
    }
}

struct watcher_file *watcher_parse_argument_file(char const *const arg, enum watcher_type type) {
    struct watcher_file *const watcher = malloc(sizeof *watcher);
    if (!watcher) {
        pr_error_with_errno("Failed to allocate memory for watcher for file '%s' type %d(%s)", arg, type, watcher_type_strings[type]);
        return NULL;
    }
    if (snprintf(watcher->path, PATH_MAX, "%s%s", watcher_file_prefix_table[type], arg) < 0) {
        pr_error_with_errno("Failed to store/generate path for watcher for file '%s' type %d(%s)", arg, type, watcher_type_strings[type]);
        free(watcher);
        return NULL;
    }
    return watcher;
}

struct watcher_net_carrier *watcher_parse_argument_net_carrier(char const *const arg) {
    struct watcher_net_carrier *const watcher = malloc(sizeof *watcher);
    if (!watcher) {
        pr_error_with_errno("Failed to llocate memory for net carrier for interface '%s'", arg);
        return NULL;
    }
    // char path[PATH_MAX];
    strncpy(watcher->interface, arg, sizeof watcher->interface);
    return watcher;
}

int watcher_init_net_carrier(struct watcher *const watcher) {
    char path[PATH_MAX];
    if (snprintf(path, PATH_MAX, "/sys/class/net/%s/carrier", watcher->net_carrier->interface) < 0) {
        pr_error_with_errno("Failed to generate path for network carrier watcher for interface '%s'\n", watcher->net_carrier->interface);
        return 1;
    }
    if ((watcher->net_carrier->carrier_fd = open(path, O_RDONLY)) < 0) {
        pr_error_with_errno("Failed to open carrier stat file for interface '%s'\n", watcher->net_carrier->interface);
        return 2;
    }
    return 0;
}

int watcher_init(struct watcher *const watcher){
    if (watcher->type == WATCHER_TYPE_NET_CARRIER) {
        if (watcher_init_net_carrier(watcher)) {
            pr_error("Failed to init net carrier watcher\n");
            return 1;
        }
    }
    watcher->dots_mask = openvfd_lookup_dots(watcher->dots_type);
    pr_debug("Prepared a watcher of type %d(%s), dots type %d, mask 0x%x\n", watcher->type, watcher_type_strings[watcher->type], watcher->dots_type, watcher->dots_mask);
    return 0;
}

int watcher_prepare(struct watcher *const watcher_head) {
    for (struct watcher *watcher = watcher_head; watcher; watcher = watcher->next) {
        if (watcher_init(watcher)) {
            pr_error("Failed to init watcher for type %d(%s)\n", watcher->type, watcher_type_strings[watcher->type]);
            return 1;
        }
    }
    return 0;
}

struct watcher *wacher_parse_argument(char const *const arg) {
    if (!arg || !arg[0] || arg[0] != '@') {
        pr_error("Argument not set or invalid\n");
        return NULL;
    }
    char const *seps[WATCHER_PARSE_ARGUMENT_MAX_SEP] = {NULL};
    char const *end = NULL;
    unsigned sep_id = 0;
    for (char const *c = arg; !end; ++c) {
        switch (*c) {
        case ':':
            if (sep_id < WATCHER_PARSE_ARGUMENT_MAX_SEP) {
                seps[sep_id++] = c;
            }
            break;
        case '\0':
            end = c;
            break;
        default:
            break;
        }
    }
    if (sep_id < WATCHER_PARSE_ARGUMENT_MAX_SEP) {
        pr_error("Argument too short, expecting at least 1 seperator: '%s'\n", arg);
        return NULL;
    }
    if (!end) {
        pr_error("Argument not correctly ended: '%s'\n", arg);
        return NULL;
    }
    char temp[WATCHER_PARSE_ARGUMENT_MAX_LEN];
    size_t len = watcher_parse_argument_safe_len(seps[1] - seps[0] - 1);
    strncpy(temp, seps[0] + 1, len);
    temp[len] = '\0';
    enum watcher_type const watcher_type = watcher_get_type_from_string(temp);
    if (!watcher_type) {
        pr_error("Argument does not define valid watcher type: '%s'\n", arg);
        return NULL;
    }
    enum dots_type dots_type;
    len = watcher_parse_argument_safe_len(seps[0] - arg - 1);
    strncpy(temp, arg + 1, len);
    temp[len] = '\0';
    if (!(dots_type = dots_get_type_from_string(temp))) {
        pr_error("Argument does not define valid dots type: '%s'\n", arg);
        return NULL;
    }
    struct watcher *watcher = malloc(sizeof *watcher);
    if (!watcher) {
        pr_error_with_errno("Failed to allocate memory for watcher");
        return NULL;
    }
    switch (watcher_type) {
        case WATCHER_TYPE_FILE:
        case WATCHER_TYPE_DEV:
        case WATCHER_TYPE_SYS:
            watcher->file = watcher_parse_argument_file(seps[1] + 1, watcher_type);
            break;
        case WATCHER_TYPE_NET_CARRIER:
            watcher->net_carrier = watcher_parse_argument_net_carrier(seps[1] + 1);
            break;
        default:
            free(watcher);
            return NULL;
    }
    if (!watcher->raw) {
        pr_error("Failed to parse argument into watcher definition\n");
        return NULL;
    }
    watcher->next = NULL;
    watcher->type = watcher_type;
    watcher->dots_type = dots_type; 
    return watcher;
}

int watcher_check_file(struct watcher *const watcher, uint8_t *dots) {
    struct stat st;
    if (!stat(watcher->file->path, &st)) {
        *dots |= watcher->dots_mask;
    }
    return 0;
}

int watcher_check_net_carrier(struct watcher *const watcher, uint8_t *dots) {
    char buffer[16];
    buffer[0] = '\0';
    if (read(watcher->net_carrier->carrier_fd, buffer, sizeof buffer) < 0) {
        pr_error_with_errno("Failed to read from carrier stat file for interface '%s'\n", watcher->net_carrier->interface);
        return 1;
    }
    if (buffer[0] == '1') {
        *dots |= watcher->dots_mask;
    }
    return 0;
}

int watcher_check_all(struct watcher *const watcher_head, uint8_t *const dots) {
    for (struct watcher *watcher = watcher_head; watcher; watcher = watcher->next) {
        int r = 0;
        switch (watcher->type) {
            case WATCHER_TYPE_FILE:
            case WATCHER_TYPE_DEV:
            case WATCHER_TYPE_SYS:
                r = watcher_check_file(watcher, dots);
                break;
            case WATCHER_TYPE_NET_CARRIER:
                r = watcher_check_net_carrier(watcher, dots);
                break;
            default:
                pr_error("Watcher type invalid: %d\n", watcher->type);
                return 1;
        }
        if (r) {
            pr_error("Failed to check watcher status\n");
            return 1;
        }
    }
    return 0;
}
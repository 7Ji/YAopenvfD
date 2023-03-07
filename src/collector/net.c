#include "collector/net.h"

struct collector_net *collector_parse_argument_net(char const *const sep_type_iface, char const *const sep_iface_tr, char const *const end) {
    if (sep_iface_tr <= sep_type_iface + 1) {
        pr_error("Interface device is not defined\n");
        return NULL;
    }
    if (end <= sep_iface_tr + 1) {
        pr_error("Tx/Rx type is not defined");
        return NULL;
    }
    enum collector_net_type type;
    switch (*(sep_iface_tr + 1)) {
    case 't':
    case 'T':
        type = COLLECTOR_NET_TYPE_TX;
        break;
    case 'r':
    case 'R':
        type = COLLECTOR_NET_TYPE_RX;
        break;
    case 'm':
    case 'M':
        type = COLLECTOR_NET_TYPE_MIXED;
        break;
    default:
        pr_error("Tx/Rx type illegal: %c, should be either one of t, T, r, R, m, M\n", *(sep_iface_tr + 1));
        return NULL;
    }
    size_t len_iface = sep_iface_tr - sep_type_iface - 1;
    struct collector_net *collector;
    if (len_iface >= sizeof collector->interface) {
        pr_error("Network interface  name too long, length %lu but at most %lu: '%s'\n", len_iface, sizeof collector->interface, sep_type_iface + 1);
        return NULL;
    }
    if (!(collector = malloc(sizeof *collector))) {
        pr_error_with_errno("Failed to allocate memory for network collector");
        return NULL;
    }
    strncpy(collector->interface, sep_type_iface + 1, len_iface);
    collector->interface[len_iface] = '\0';
    collector->type = type;
    return collector;
}

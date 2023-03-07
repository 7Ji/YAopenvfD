#include "collector/net.h"
#include "collector/size.h"
#include <fcntl.h>

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
    collector->tx_parse = type != COLLECTOR_NET_TYPE_RX;
    collector->rx_parse = type != COLLECTOR_NET_TYPE_TX;
    return collector;
}

int collector_net_init(struct collector_net *const collector) {
    char path[PATH_MAX];
    int len = snprintf(path, PATH_MAX, "/sys/class/net/%s/statistics/tx_bytes", collector->interface);
    if (len < 0) {
        pr_error_with_errno("Failed to generate sysfs tx bytes stat file for interface '%s'", collector->interface);
        return 1;
    }
    if (collector->tx_parse && (collector->tx_fd = open(path, O_RDONLY)) < 0) {
        pr_error_with_errno("Failed to open '%s' for read Tx bytes for interface '%s'", path, collector->interface);
        return 2;
    }
    path[len - 8] = 'r';
    if (collector->rx_parse && (collector->rx_fd = open(path, O_RDONLY)) < 0) {
        pr_error_with_errno("Failed to open '%s' for read Rx bytes for interface '%s'", path, collector->interface);
        close(collector->tx_fd);
        return 3;
    }
    return 0;
}

int collector_net_parse(struct collector_net *const collector) {
    char buffer[64];
    buffer[0] = '\0';
    if (collector->tx_parse) {
        if (lseek(collector->tx_fd, 0, SEEK_SET) < 0) {
            pr_error_with_errno("Failed to seek to head of sysfs Tx stat file for interface '%s'", collector->interface);
            return 1;
        }
        if (read(collector->tx_fd, buffer, 64) < 0) {
            pr_error_with_errno("Failed to read into buffer for parse of Tx stat for interface '%s'", collector->interface);
            return 2;
        }
        collector->tx_this = strtoul(buffer, NULL, 10);
        buffer[0] = '\0';
    }
    if (collector->rx_parse) {
        if (lseek(collector->rx_fd, 0, SEEK_SET) < 0) {
            pr_error_with_errno("Failed to seek to head of sysfs Rx stat file for interface '%s'", collector->interface);
            return 3;
        }
        if (read(collector->rx_fd, buffer, 64) < 0) {
            pr_error_with_errno("Failed to read into buffer for parse of Rx stat for interface '%s'", collector->interface);
            return 4;
        }
        collector->rx_this = strtoul(buffer, NULL, 10);
    }
    return 0;
}

int collector_net_prepare(struct collector_net *const collector) {
    if (collector_net_parse(collector)) {
        pr_error("Failed to prepare collector for network for interface '%s'\n", collector->interface);
        return 1;
    }
    return 0;
}

int collector_net_report(struct collector_net *const collector, char report[COLLECTOR_REPORT_SIZE]) {
    collector->tx_last = collector->tx_this;
    collector->rx_last = collector->rx_this;
    if (collector_net_parse(collector)) {
        pr_error("Failed to parse network interface stat for '%s' for report\n", collector->interface);
        return 1;
    }
    unsigned long diff = 0;
    if (collector->tx_parse) {
        diff = diff + collector->tx_this - collector->tx_last;
    }
    if (collector->rx_parse) {
        diff = diff + collector->rx_this - collector->rx_last;
    }
    char suffix;
    diff = collector_size_to_human_readable(diff, &suffix);
    if (snprintf(report, COLLECTOR_REPORT_SIZE, "%3lu%c", diff, suffix) < 0) {
        pr_error_with_errno("Failed to generate report string for human-readable size %lu%c\n", diff, suffix);
        return 3;
    }
    return 0;
}
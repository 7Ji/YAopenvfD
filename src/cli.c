#include "cli.h"
#include "reporter.h"
#include "version.h"

static inline void cli_help() {
    puts(
        "\n"
        "YAopenvfD [reporter] ([reporter] ([reporter] (...))) [watcher] ([watcher] ([watcher])) (--help) (--version)\n\n"
        "  [reporter]:\t[duration](@[dot]):[type](:[definer])\n"
        "\t\t  define a reporter that should be displayed for a certain duration, during which optionally keep a dot on\n"
        "    [duration]:\tduration of this reporter, in second, non-negative (0 for forever)\n"
        "    [dot]:\tname of the dot that should be kept on during this reporter (e.g. usb, eth, etc)\n"
        "    [type]:\tone of the following (and corresponding definer):\n"
        "      string:\tdisplay string in definer, scroll if too long\n"
        "      temp:\tdisplay temp in thermal zone N defined in definer, in celcius\n"
        "      io: \tdisplay a type of I/O speed of block device defined in definer\n"
        "\t[io definer]:\t[block device]:[type]\n"
        "\t  [block device]:\tthe name of block device, e.g. mmcblk2\n"
        "\t  [type]:\tr or R (for read), w or W (for write), m or M (for mixed)\n"
        "      cpu: \tdisplay whole CPU load, or optionally for a specific core no. in definer if valid\n"
        "      net: \tdisplay a type of network TX/RX on a specific interface\n"
        "\t[net definer]:\t[interface]:[type]\n"
        "\t  [interface]:\tthe name of block device, e.g. mmcblk2\n"
        "\t  [type]:\tt or T (for Tx/transmit), r or R (for Rx/receive)\n"
        "\n"
        "  [watcher]:\t@[dot]:[type](:definer)\n"
        "\t\t define a watcher that turn on/off a dot on hotplug\n"
        "    [dot]:\tsame definition as in [reporter]\n"
        "    [type]:\tone of the following (and corresponding definer):\n"
        "      file:\twatch for the 'file' (could be non-regular file) defined in definer that could appear/disappear, definer e.g. /tmp/myfile, /var/lock/mysql.lock, etc\n"
        "      dev:\tshort hand for file under /dev, definer e.g. sda, disk/by-id/nvme-eui.e8238fa6bf530001001b448b498949f8, etc\n"
        "      sys:\tshort hand for file under /sys, definer e.g. class/input/mouse0, etc\n"
        "      net:\twatch for carrier status of a network interface, definer e.g. eth0, wlan0, etc\n"
    );
}

static inline void cli_version() {
    puts(version());
}

int cli_parse(int const argc, char const *const argv[], struct reporter **const reporter_head) {
    if (!argv) {
        pr_error("Varadic arguments empty\n");
        return 1;
    }
    if (argc <= 1) {
        pr_warn("Arguments too few!\n");
        cli_help();
        return 2;
    }
    for (int i = 0; i < argc; ++i) {
        if (argv[i][0] == '-' && argv[i][1] == '-') {
            char const *const scmp = argv[i] + 2;
            if (!strncmp(scmp, "help", 4)) {
                cli_help();
                return -1;
            }
            if (!strncmp(scmp, "version", 4)) {
                cli_version();
                return -1;
            }
        }
    }
    struct reporter *reporter_last = NULL;
    struct reporter *reporter_current;
    for (int i = 0; i < argc; ++i) {
        switch(argv[i][0]) {
        case '0'...'9':
            if (!(reporter_current = reporter_parse_argument(argv[i]))) {
                pr_error("Failed to parse reporter definition '%s'\n", argv[i]);
                return 3;
            }
            if (!*reporter_head) {
                *reporter_head = reporter_current;
            }
            if (reporter_last) {
                reporter_last->next = reporter_current;
            }
            reporter_last = reporter_current;
        default:
            break;
        }
    }
    if (*reporter_head && reporter_last) {
        return 0;
    } else {
        pr_error("No reporter defined\n");
        return 4;
    }
}

int cli_interface(int const argc, char const *const argv[]) {
    struct reporter *reporter_head = NULL;
    int r = cli_parse(argc, argv, &reporter_head);
    if (r > 0) {
        pr_error("Failed to parse arguments\n");
        return 1;
    }
    if (r < 0) {
        return 0;
    }
    if (!reporter_head) {
        pr_error("No reporter defined\n");
        return 2;
    }
    if ((r = reporter_prepare(reporter_head))) {
        pr_error("Failed to prepare reporters: %d\n", r);
        return 3;
    }
    if ((r = reporter_loop(reporter_head))) {
        pr_error("Reporter loop breaks: %d\n", r);
        return 4;
    }
    pr_warn("Unexpected exit point, but allow it for now\n");
    return 0;
}
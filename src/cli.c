#include "cli.h"
#include "collector.h"
#include "reporter.h"
#include "version.h"

static inline void cli_help() {
    puts(
        "\n"
        "YAopenvfD [reporter] ([reporter] ([reporter] (...))) (--help) (--version)\n\n"
        "  [reporter]\t[duration]:[type](:[definer])\n"
        "    [duration]\tduration of this reporter, in second, positive\n"
        "\t\tspecial value 0 means it lasts forever (remaining reporters will be ignored)\n"
        "    [type]\tone of the following (and corresponding definer):\n"
        "\t\tstring:\tdisplay string in definer, scroll if too long\n"
        "\t\ttemp:\tdisplay temp in thermal zone N defined in definer, in celcius, defaults to 0\n"
        "\t\tio:\tdisplay I/O of block device defined in definer (e.g. mmcblk2)\n"
        "\t\t\toptional suffix: a string start with :, then combined of r (for read), w (for write), m (for mixed)\n"
        "\t\t\tloop through all types (could be duplicated) in it, default is rwm\n"
        "\t\tcpu:\tdisplay read-time CPU load\n"
        "\t\tnet:\tdisplay network TX/RX\n"
    );
}

static inline void cli_version() {
    puts(version());
}

int cli_parse(int argc, char *argv[]) {
    if (!argv) {
        pr_error("Varadic arguments empty\n");
        return -1;
    }
    if (argc <= 1) {
        pr_warn("Arguments too few!\n");
        cli_help();
        return 1;
    }
    for (int i = 0; i < argc; ++i) {
        if (argv[i][0] == '-' && argv[i][1] == '-') {
            char *const scmp = argv[i] + 2;
            if (!strncmp(scmp, "help", 4)) {
                cli_help();
                return 0;
            }
            if (!strncmp(scmp, "version", 4)) {
                cli_version();
                return 0;
            }
        }
    }
    struct reporter *reporter_head = NULL;
    struct reporter *reporter_last = NULL;
    struct reporter *reporter_current;
    for (int i = 0; i < argc; ++i) {
        switch(argv[i][0]) {
        case '0'...'9':
            if (!(reporter_current = reporter_parse_argument(argv[i]))) {
                pr_error("Failed to parse reporter definition '%s'\n", argv[i]);
                return 2;
            }
            if (!reporter_head) {
                reporter_head = reporter_current;
            }
            if (reporter_last) {
                reporter_last->next = reporter_current;
            } else {
                reporter_last = reporter_current;
            }
        default:
            break;
        }
    }
    if (reporter_head && reporter_last) {
        return 0;
    } else {
        pr_error("No reporter defined\n");
        return 3;
    }
}

int cli_interface(int argc, char *argv[]) {
    int r = cli_parse(argc, argv);
    if (r) {
        return r;
    }
    return 0;
}
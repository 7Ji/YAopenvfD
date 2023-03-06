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

// static inline int cli_parse_each(struct collectors *collectors, struct collectors *collectors_last, char *arg) {
//     char *seps[3];
//     unsigned sep_id = 0;
//     for (char *c = arg;; ++c) {
//         switch (*c) {
//             case ':':
//                 seps[sep_id++] = c;
//                 if (sep_id == 3) {
//                     break;
//                 }
//             case '\0': {
//                 if (sep_id < 1) {
//                     pr_error("Argument too short: %s\n", arg);
//                     return 1;
//                 }
//                 enum reporter_type reporter_type = REPORTER_TYPE_NONE;
//                 char temp[128];
//                 size_t len = cli_parse_argument_safe_len(seps[1] - seps[0] - 1);
//                 strncpy(temp, seps[0] + 1, len);
//                 temp[len] = '\0';
//                 if (!strncmp(temp, "string", 7)) {
//                     reporter_type = REPORTER_TYPE_STRING;
//                 } else if (!strncmp(temp, "temp", 5)) {
//                     reporter_type = REPORTER_TYPE_TEMP;
//                 } else if (!strncmp(temp, "io", 3)) {
//                     reporter_type = REPORTER_TYPE_IO;
//                 } else if (!strncmp(temp, "cpu", 4)) {
//                     reporter_type = REPORTER_TYPE_CPU;
//                 } else if (!strncmp(temp, "net", 4)) {
//                     reporter_type = REPORTER_TYPE_NET;
//                 } else {
//                     pr_error("Argument does not define valid reporter type: %s\n", arg);
//                     return 2;
//                 }
//                 len = cli_parse_argument_safe_len(seps[0] - arg);
//                 strncpy(temp, arg, len);
//                 temp[len] = '\0';
//                 unsigned long duration = strtoul(temp, NULL, 10);
//                 pr_warn("Reporter for %s duration %lu\n", reporter_get_type_string(reporter_type), duration);
//                 return 0;
//             }
//         }
//     }
//     return -1;
// }

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
    for (int i = 0; i < argc; ++i) {
        switch(argv[i][0]) {
        case '0'...'9': {
            
        }
        default:
            break;
        }
    }
    return 0;
}

int cli_interface(int argc, char *argv[]) {
    int r = cli_parse(argc, argv);
    if (r) {
        return r;
    }
    return 0;
}
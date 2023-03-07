#include "collector.h"
#include "collector/string.h"
#include "collector/temp.h"
#include "collector/io.h"
#include "collector/cpu.h"
#include "collector/net.h"

#include <fcntl.h>

static const unsigned collector_parse_argument_minium_sep_count[] = {
    (unsigned)-1, // None
    2,  // String
    2,  // Temp
    3,  // IO
    1,  // CPU
    3,  // Net
};

int collector_init(struct collector const collector) {
    switch (collector.type) {
    case COLLECTOR_TYPE_STRING:
        return 0;
    case COLLECTOR_TYPE_TEMP:
        return collector_temp_init(collector.temp);
    case COLLECTOR_TYPE_IO:
        return collector_io_init(collector.io);
    case COLLECTOR_TYPE_CPU:
        return collector_cpu_init(collector.cpu);
    case COLLECTOR_TYPE_NET:
        return 0;
    default:
        pr_error("Unexpected collector type %d\n", collector.type);
        return -1;
    }
}

int collector_prepare(struct collector const collector) {
    switch (collector.type) {
    case COLLECTOR_TYPE_STRING:
        return collector_string_prepare(collector.string);
    case COLLECTOR_TYPE_TEMP:
        return 0;
    case COLLECTOR_TYPE_IO:
        return collector_io_prepare(collector.io);
    case COLLECTOR_TYPE_CPU:
        return collector_cpu_prepare(collector.cpu);
    case COLLECTOR_TYPE_NET:
        return 0;
    default:
        pr_error("Unexpected collector type %d\n", collector.type);
        return -1;
    }
}

int collector_report(struct collector const collector, char report[5]) {
    switch (collector.type) {
    case COLLECTOR_TYPE_STRING:
        return collector_string_report(collector.string, report);
    case COLLECTOR_TYPE_TEMP:
        return collector_temp_report(collector.temp, report);
    case COLLECTOR_TYPE_IO:
        return collector_io_report(collector.io, report);
    case COLLECTOR_TYPE_CPU:
        return collector_cpu_report(collector.cpu, report);
    case COLLECTOR_TYPE_NET:
        return 0;
    default:
        pr_error("Unexpected collector type %d\n", collector.type);
        return -1;
    }
}

int collector_parse_argument(struct collector *collector, char const *const arg, char const *const seps[], unsigned const sep_count, char const *const end) {
    if (!collector) {
        pr_error("Collector not allocated yet\n");
        return -1;
    }
    if (sep_count < collector_parse_argument_minium_sep_count[collector->type]) {
        pr_error("Argument too short, expecting at least %u seperators but only %u: '%s'\n", collector_parse_argument_minium_sep_count[collector->type], sep_count, arg);
        return 1;
    }
    switch (collector->type) {
    case COLLECTOR_TYPE_STRING:
        collector->string = collector_parse_argument_string(seps[1] + 1, end);
        break;
    case COLLECTOR_TYPE_TEMP:
        collector->temp = collector_parse_argument_temp(seps[1] + 1, end);
        break;
    case COLLECTOR_TYPE_IO:
        collector->io = collector_parse_argument_io(seps[1], seps[2], end);
        break;
    case COLLECTOR_TYPE_CPU:
        collector->cpu = collector_parse_argument_cpu(seps[1]);
        break;
    case COLLECTOR_TYPE_NET:
        collector->net = collector_parse_argument_net(seps[1], seps[2], end);
        break;
    default:
        pr_error("Collector type not set: '%s'\n", arg);
        return 1;
    }
    if (!collector->raw) {
        pr_error("Failed to parse argument to collector: '%s'\n", arg);
        return 2;
    }
    return 0;
}
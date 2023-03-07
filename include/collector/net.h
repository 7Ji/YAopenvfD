#ifndef _HAVE_COLLECTOR_NET_H
#define _HAVE_COLLECTOR_NET_H
#include "collector.h"
struct collector_net *collector_parse_argument_net(char const *sep_type_iface, char const *sep_iface_tr, char const *end);
int collector_net_init(struct collector_net *collector);
int collector_net_prepare(struct collector_net *collector);
int collector_net_report(struct collector_net *collector, char report[COLLECTOR_REPORT_SIZE]);
#endif
/*
YAopenvfD/src/collector/date.c: logic to collect date/time
Copyright (C) 2023 Guoxin "7Ji" Pu <pugokushin@gmail.com>

This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with this program. If not, see <https://www.gnu.org/licenses/>.
*/

#include "collector/date.h"
#include <time.h>

#define COLLECTOR_DATE_TYPE_MAX COLLECTOR_DATE_TYPE_WEEKDAY

static const char collector_date_type_strings[][11] = {
    "",
    "24h",
    "12h",
    "year",
    "year_month",
    "month_year",
    "month_day",
    "day_month",
    "weekday"
};

struct collector_date *collector_parse_argument_date(char const *const start, char const *const end) {
    if (start >= end) {
        pr_error("Date collector has empty string\n");
        return NULL;
    }
    size_t const len = end - start;
    struct collector_date *collector;
    enum collector_date_type type;
    for (type = COLLECTOR_DATE_TYPE_NONE + 1; type <= COLLECTOR_DATE_TYPE_MAX; ++type) {
        if (!strncmp(collector_date_type_strings[type], start, len) && collector_date_type_strings[type][len] == '\0') {
            break;
        }
    }
    if (type > COLLECTOR_DATE_TYPE_MAX) {
        pr_error("'%s' is not a valid date collector definer\n", start);
        return NULL;
    }
    if (!(collector = malloc(sizeof *collector))) {
        pr_error_with_errno("Failed to allocate memory for date collector");
        return NULL;
    }
    switch (collector->type = type) {
    case COLLECTOR_DATE_TYPE_24H:
    case COLLECTOR_DATE_TYPE_12H:
        collector->blink = true;
        break;
    case COLLECTOR_DATE_TYPE_YEAR:
    case COLLECTOR_DATE_TYPE_YEAR_MONTH:
    case COLLECTOR_DATE_TYPE_MONTH_YEAR:
    case COLLECTOR_DATE_TYPE_MONTH_DAY:
    case COLLECTOR_DATE_TYPE_DAY_MONTH:
    case COLLECTOR_DATE_TYPE_WEEKDAY:
        collector->blink = false;
        break;
    default:
        pr_error("Date collector type not defined, this should not happen");
        free(collector);
        return NULL;
    }
    return collector;
}

int collector_date_report(struct collector_date *const collector, char report[COLLECTOR_REPORT_SIZE]) {
    time_t time_now =  time(NULL);
    if (time_now == (time_t) -1) {
        pr_error_with_errno("Failed to get current unix timestamp");
        return 1;
    }
    struct tm tm_now;
    if (!localtime_r(&time_now, &tm_now)) {
        pr_error_with_errno("Failed to get local time struct");
        return 2;
    }
    int r = 0;
    switch (collector->type) {
    case COLLECTOR_DATE_TYPE_24H:
        r = snprintf(report, COLLECTOR_REPORT_SIZE, "%02d%02d", tm_now.tm_hour, tm_now.tm_min);
        break;
    case COLLECTOR_DATE_TYPE_12H:
        r = snprintf(report, COLLECTOR_REPORT_SIZE, "%02d%02d", tm_now.tm_hour % 12, tm_now.tm_min);
        break;
    case COLLECTOR_DATE_TYPE_YEAR:
        r = snprintf(report, COLLECTOR_REPORT_SIZE, "%04d", tm_now.tm_year + 1900);
        break;
    case COLLECTOR_DATE_TYPE_YEAR_MONTH:
        r = snprintf(report, COLLECTOR_REPORT_SIZE, "%02d%02d", (tm_now.tm_year + 1900) % 1000, tm_now.tm_mon + 1);
        break;
    case COLLECTOR_DATE_TYPE_MONTH_YEAR:
        r = snprintf(report, COLLECTOR_REPORT_SIZE, "%02d%02d", tm_now.tm_mon + 1, (tm_now.tm_year + 1900) % 1000);
        break;
    case COLLECTOR_DATE_TYPE_MONTH_DAY:
        r = snprintf(report, COLLECTOR_REPORT_SIZE, "%02d%02d", tm_now.tm_mon + 1, tm_now.tm_mday);
        break;
    case COLLECTOR_DATE_TYPE_DAY_MONTH:
        r = snprintf(report, COLLECTOR_REPORT_SIZE, "%02d%02d", tm_now.tm_mday, tm_now.tm_mon + 1);
        break;
    case COLLECTOR_DATE_TYPE_WEEKDAY:
        switch (tm_now.tm_wday) {
            case 0:
                strncpy(report, "SUN", COLLECTOR_REPORT_SIZE);
                break;
            case 1:
                strncpy(report, "MON", COLLECTOR_REPORT_SIZE);
                break;
            case 2:
                strncpy(report, "TUE", COLLECTOR_REPORT_SIZE);
                break;
            case 3:
                strncpy(report, "WED", COLLECTOR_REPORT_SIZE);
                break;
            case 4:
                strncpy(report, "THU", COLLECTOR_REPORT_SIZE);
                break;
            case 5:
                strncpy(report, "FRI", COLLECTOR_REPORT_SIZE);
                break;
            case 6:
                strncpy(report, "SAT", COLLECTOR_REPORT_SIZE);
                break;
            default:
                pr_error("Unexpected week day: %d\n", tm_now.tm_wday);
                return 3;
        }
        r = snprintf(report + 3, 2, "%1d", tm_now.tm_wday);
        break;
    default:
        pr_error("Date collector type not defined, this should not happen\n");
        return 4;
    }
    if (r < 0) {
        pr_error("Failed to report date\n");
        return 5;
    }
    return 0;
}
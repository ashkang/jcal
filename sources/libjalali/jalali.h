/*
 * jalali.h - Tools for manipulating Jalali representation of Iranian calendar
 * and necessary conversations to Gregorian calendar.
 * Copyright (C) 2006, 2007, 2009, 2010, 2011 Ashkan Ghassemi.
 *
 * This file is part of libjalali.
 *
 * libjalali is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * libjalali is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.      See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with libjalali.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef JALALI_H
#define JALALI_H

#include <time.h>

#ifdef __cplusplus
extern "C" {
#if 0 /* /me mutters something about emacs. */
}
#endif
#endif

#define LIBJALALI_VERSION "0.4.1gitd9200f"

struct jtm {
    int tm_sec;            /* Seconds. (0-59) */
    int tm_min;            /* Minutes. (0-59) */
    int tm_hour;           /* Hours. (0-59) */
    int tm_mday;           /* Day of the month. (1-31) */
    int tm_mon;            /* Month. (0-11) */
    int tm_year;           /* Year. */
    int tm_wday;           /* Day of the week. (0-6) */
    int tm_yday;           /* Day in the year. (0-365) */
    int tm_isdst;          /* Daylight saving time is in effect. */
    long int tm_gmtoff;    /* Seconds east of UTC. */
    const char *tm_zone;   /* Timezone abbreviation.  */
};

struct ab_jtm {
    int ab_sec;
    int ab_min;
    int ab_hour;
    int ab_days;
};

struct jyinfo {
    int lf;                /* leap indicator flag */
    int y;                 /* year */
    int r;                 /* reamining years in grand cycle */
    int p;                 /* passed years from grand cycle*/
    int rl;                /* remaining leap years in grand cycle */
    int pl;                /* passed leap years in grand cycle */
    int apl;               /* absolute passed leaps */
};


/* Jalali leap year indication function. */
extern int jalali_is_jleap(int year);

extern void jalali_create_time_from_secs(time_t time, struct ab_jtm* ab_jtm);

extern time_t jalali_create_secs_from_time(const struct ab_jtm* ab_jtm);

extern int jalali_create_date_from_days(struct jtm* j);

extern int jalali_create_days_from_date(struct jtm* j);

extern void jalali_get_jyear_info(struct jyinfo* jyinfo);

extern void jalali_get_date(int p, struct jtm* jtm);

extern int jalali_get_diff(const struct jtm* jtm);

extern void jalali_update(struct jtm* jtm);

extern void jalali_show_time(const struct jtm* j);

extern int jalali_year_month_days(int year, int month);

#ifdef __cplusplus
}
#endif

#endif /* JALALI_H */

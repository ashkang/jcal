/*
 * jtime.c - Tools for manipulating Jalali representation of Iranian calendar
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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with libjalali.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/time.h>
#include "jconfig.h"
#include "jalali.h"
#include "jtime.h"

const char* GMT_ZONE = "UTC";
const char* GMT_ZONE_fa = "گرینویچ";
const char* jalali_months[] = { "Farvardin", "Ordibehesht", "Khordaad",
                                "Tir", "Mordaad", "Shahrivar", "Mehr",
                                "Aabaan", "Aazar", "Dey", "Bahman", "Esfand" };
const char* fa_jalali_months[] = { "فروردین", "اردیبهشت", "خرداد",
                                   "تیر", "مرداد", "شهریور",
                                   "مهر", "آبان", "آذر",
                                   "دی", "بهمن", "اسفند" };
const char* jalali_months_3[] = { "Far", "Ord", "Kho", "Tir", "Mor", "Sha",
                                  "Meh", "Aba", "Aza", "Dey", "Bah", "Esf" };
const char* fa_jalali_months_3[] = { "فرو", "ارد", "خرد", "تیر", "مرد", "شهر",
                                     "مهر", "آبا", "آذر", "دی ", "بهم", "اسف"};
const char* jalali_days_fa[] = { "Shanbeh", "Yek-Shanbeh", "Do-Shanbeh",
                                 "Seh-Shanbeh", "Chahaar-Shanbeh",
                                 "Panj-Shanbeh", "Jomeh" };
const char* fa_jalali_days[] = { "شنبه", "یکشنبه", "دوشنبه", "سه شنبه",
                                 "چهارشنبه", "پنجشنبه", "جمعه" };
const char* jalali_days_3_fa[] = { "Sha", "Yek", "Dos", "Ses", "Cha", "Pan",
                                   "Jom" };
const char* fa_jalali_days_3[] = { "شنب", "یکش", "دوش", "سهش", "چها", "پنج",
                                   "جمع" };
const char* jalali_days_2_fa[] = { "Sh", "Ye", "Do", "Se", "Ch", "Pa", "Jo" };

const char* fa_jalali_days_2[] = { "شن", "یک", "دو", "سه", "چه", "پن", "جم" };
const char* jalali_days[] = { "Saturday", "Sunday", "Monday", "Tuesday",
                              "Wednesday", "Thursday", "Friday" };
const char* jalali_days_3[] = { "Sat", "Sun", "Mon", "Tue", "Wed", "Thu",
                                "Fri" };
const char* jalali_days_2[] = { "Sa", "Su", "Mo", "Tu", "We", "Th", "Fr" };
const char* farsi_digits[] = { "۰", "۱", "۲", "۳", "۴",
                               "۵", "۶", "۷", "۸", "۹" };

const char* tzname_fa[2] = { "زمان زمستانی", "زمان تابستانی" };

static char in_buf[MAX_BUF_SIZE] = {0};
static struct jtm in_jtm;

extern char* tzname[2];
extern const int jalali_month_len[];

void in_jasctime(const struct jtm* jtm, char* buf)
{
    if (!jtm)
        return;

    if (jtm->tm_wday < 0 || jtm->tm_wday > 6)
        return;

    if (jtm->tm_mon < 0 || jtm->tm_mon > 11)
        return;

    if (jtm->tm_mday < 1 || jtm->tm_mday > 31)
        return;

    if (buf) {
        sprintf(buf, "%s %s %02d %02d:%02d:%02d %d\n",
                jalali_days_3_fa[jtm->tm_wday], jalali_months_3[jtm->tm_mon],
                jtm->tm_mday, jtm->tm_hour, jtm->tm_min, jtm->tm_sec,
                jtm->tm_year);
    } else {
        snprintf(in_buf, MAX_BUF_SIZE, "%s %s %02d %02d:%02d:%02d %d\n",
                 jalali_days_3_fa[jtm->tm_wday], jalali_months_3[jtm->tm_mon],
                 jtm->tm_mday, jtm->tm_hour, jtm->tm_min, jtm->tm_sec,
                 jtm->tm_year);

    }
}

void in_jlocaltime(const time_t* timep, struct jtm* result)
{
    if (!timep)
        return;

    struct tm t;
    struct jtm c_jtm;
    struct ab_jtm ab;
    long int gmtoff;
    time_t c;

    tzset();

    localtime_r(timep, &t);

#if defined _WIN32 || defined __MINGW32__ || defined __CYGWIN__
    struct timeval tv;
    struct timezone tz;

    gettimeofday(&tv, &tz);
    gmtoff = (-tz.tz_minuteswest) * J_MINUTE_LENGTH_IN_SECONDS +
        (tz.tz_dsttime * J_HOUR_LENGTH_IN_SECONDS);
    c_jtm.tm_zone = tzname[t.tm_isdst];
#else
    gmtoff = t.tm_gmtoff;
    c_jtm.tm_zone = t.tm_zone;
#endif

    c = (*timep) + (time_t) gmtoff;

    jalali_create_time_from_secs(c, &ab);
    jalali_get_date(ab.ab_days, &c_jtm);
    jalali_create_date_from_days(&c_jtm);
    c_jtm.tm_sec = ab.ab_sec;
    c_jtm.tm_min = ab.ab_min;
    c_jtm.tm_hour = ab.ab_hour;
    c_jtm.tm_isdst = t.tm_isdst;

    c_jtm.tm_gmtoff = gmtoff;
    memcpy(result ? result : &in_jtm, &c_jtm, sizeof(struct jtm));
}

void in_jctime(const time_t* timep, char* buf)
{
    if (!timep)
        return;

    struct jtm c_jtm;
    in_jlocaltime(timep, &c_jtm);

    in_jasctime(&c_jtm, buf ? buf : 0);
}

void in_jgmtime(const time_t* timep, struct jtm* result)
{
    if (!timep)
        return;

    struct tm t;
    struct jtm c_jtm;
    struct ab_jtm ab;
    time_t c;
    tzset();

    gmtime_r(timep, &t);
    c = *timep;

    jalali_create_time_from_secs(c, &ab);
    jalali_get_date(ab.ab_days, &c_jtm);
    jalali_create_date_from_days(&c_jtm);
    c_jtm.tm_sec = ab.ab_sec;
    c_jtm.tm_min = ab.ab_min;
    c_jtm.tm_hour = ab.ab_hour;
    c_jtm.tm_isdst = 0;

    c_jtm.tm_zone = GMT_ZONE;
    c_jtm.tm_gmtoff = 0;

    memcpy(result ? result : &in_jtm, &c_jtm, sizeof(struct jtm));
}

char* jasctime(const struct jtm* jtm)
{
    if (!jtm)
        return 0;

    in_jasctime(jtm, 0);

    return in_buf;
}

char* jctime(const time_t* timep)
{
    if (!timep)
        return 0;

    in_jctime(timep, 0);

    return in_buf;
}

struct jtm* jgmtime(const time_t* timep)
{
    if (!timep)
        return 0;
    in_jgmtime(timep, 0);

    return &in_jtm;
}

struct jtm* jlocaltime(const time_t* timep)
{
    if (!timep)
        return 0;

    in_jlocaltime(timep, 0);

    return &in_jtm;
}

time_t jmktime(struct jtm* jtm)
{
    if (!jtm)
        return (time_t) (-1);

    tzset();
    jalali_update(jtm);
    int p = jalali_get_diff(jtm);
    time_t t;
    t = ((time_t) p * (time_t) J_DAY_LENGTH_IN_SECONDS) +
        ((time_t) jtm->tm_hour * (time_t) J_HOUR_LENGTH_IN_SECONDS)
        + ((time_t) jtm->tm_min *
           (time_t) J_MINUTE_LENGTH_IN_SECONDS) + (time_t) jtm->tm_sec -
        ((time_t) jtm->tm_gmtoff);

    return t;
}

size_t jstrftime(char* s, size_t max, const char* format, const struct jtm* jtm)
{
    if (!s || max <= 0 || !format || !jtm)
        return -1;

    char _l1[10];
    char _l2[10];
    char _l3[10];
    char _la[100];
    char _lb[100];
    char buf[MAX_BUF_SIZE];
    int i, j;
    int fmt_n = strlen(format);
    int rb = 0;
    int b_n;
    int tmp;
    int tmp1;
    time_t t;
    struct jtm t_j;

    for (i=0; i<fmt_n && rb < (int)(max - 1); i++) {
        if (format[i] != '%') {
            s[rb] = format[i];
            rb++;
        } else {
            buf[0] = '\0';
            switch (format[i+1]) {
                /* The abbreviated weekday name. */
            case 'a':
                strncpy(buf, jalali_days_3[jtm->tm_wday], MAX_BUF_SIZE);
                break;

                /* The full weekday name. */
            case 'A':
                strncpy(buf, jalali_days[jtm->tm_wday], MAX_BUF_SIZE);
                break;

                /* The abbreviated month name. */
            case 'b':
                strncpy(buf, jalali_months_3[jtm->tm_mon], MAX_BUF_SIZE);
                break;

                /* The full month name. */
            case 'B':
                strncpy(buf, jalali_months[jtm->tm_mon], MAX_BUF_SIZE);
                break;

                /*
                 * The preferred date and time representation.
                 * example: Tue 27 Ord 1390 03:28:19 IRDT.
                 */
            case 'c':
                tzset();
                snprintf(buf, MAX_BUF_SIZE, "%s %d %s %d %02d:%02d:%02d %s",
                         jalali_days_3_fa[jtm->tm_wday], jtm->tm_mday,
                         jalali_months_3[jtm->tm_mon], jtm->tm_year,
                         jtm->tm_hour, jtm->tm_min, jtm->tm_sec,
                         jtm->tm_zone);
                break;

                /* The century number (year/100) as a 2-digit integer. */
            case 'C':
                snprintf(buf, MAX_BUF_SIZE, "%d", (jtm->tm_year / 100) + 1);
                break;

                /* The day of the month as a decimal number (range 01 to 31). */
            case 'd':
                snprintf(buf, MAX_BUF_SIZE, "%02d", jtm->tm_mday);
                break;

                /* Equivalent to %Y/%m/%d. */
            case 'D':
                snprintf(buf, MAX_BUF_SIZE, "%d/%02d/%02d", jtm->tm_year,
                         jtm->tm_mon + 1, jtm->tm_mday);
                break;

                /*
                 * Like %d, the day of the month as a decimal number, but
                 * a leading zero is replaced by a space.
                 */
            case 'e':
                snprintf(buf, MAX_BUF_SIZE, "%2d", jtm->tm_mday);
                break;

                /*
                 * The preferred date and time representation in Farsi. (utf8)
                 * example: سه شنبه ۱۷ خرداد ۱۳۹۰، ساعت ۰۸:۱۹:۲۳ (IRDT)
                 */
            case 'E':
                tzset();
                jalali_to_farsi(_l1, 10, 2, "۰", jtm->tm_hour);
                jalali_to_farsi(_l2, 10, 2, "۰", jtm->tm_min);
                jalali_to_farsi(_l3, 10, 2, "۰", jtm->tm_sec);
                jalali_to_farsi(_la, 100, 2, "۰", jtm->tm_mday);
                jalali_to_farsi(_lb, 100, 0, " ", jtm->tm_year);
                snprintf(buf, MAX_BUF_SIZE, "%s %s %s %s، ساعت %s:%s:%s - %s",
                         fa_jalali_days[jtm->tm_wday], _la,
                         fa_jalali_months[jtm->tm_mon], _lb,
                         _l1, _l2, _l3,
                         (jtm->tm_zone == GMT_ZONE) ? GMT_ZONE_fa :
                         tzname_fa[jtm->tm_isdst]);

                break;

                /*
                 * Equivalent to %Y-%m-%d (similar to the ISO 8601 date format).
                 */
            case 'F':
                snprintf(buf, MAX_BUF_SIZE, "%d-%02d-%02d", jtm->tm_year,
                         jtm->tm_mon + 1, jtm->tm_mday);
                break;

                /* The abbreviated weekday name. (Farsi-UTF8) */
            case 'g':
                strncpy(buf, fa_jalali_days_3[jtm->tm_wday], MAX_BUF_SIZE);
                break;

                /* The full weekday name. (Farsi-UTF8) */
            case 'G':
                strncpy(buf, fa_jalali_days[jtm->tm_wday], MAX_BUF_SIZE);
                break;

                /* The abbreviated month name. (Farsi-UTF8) */
            case 'v':
                strncpy(buf, fa_jalali_months_3[jtm->tm_mon], MAX_BUF_SIZE);
                break;

                /* The full month name. (Farsi-UTF8) */
            case 'V':
                strncpy(buf, fa_jalali_months[jtm->tm_mon], MAX_BUF_SIZE);
                break;

                /* The abbreviated weekday name. (Farsi) */
            case 'h':
                strncpy(buf, jalali_days_3_fa[jtm->tm_wday], MAX_BUF_SIZE);
                break;

                /* The full weekday name. (Farsi) */
            case 'q':
                strncpy(buf, jalali_days_fa[jtm->tm_wday], MAX_BUF_SIZE);
                break;

                /*
                 * The hour as a decimal number using a 24-hour clock
                 * (range 00 to 23).
                 */
            case 'H':
                snprintf(buf, MAX_BUF_SIZE, "%02d", jtm->tm_hour);
                break;

                /*
                 * The hour as a decimal number using a 12-hour clock
                 * (range 01 to 12).
                 */
            case 'I':
                snprintf(buf, MAX_BUF_SIZE, "%02d", (jtm->tm_hour == 12) ? 12 :
                         jtm->tm_hour % 12);
                break;

                /*
                 * The day of the year as a decimal number
                 * (range 001 to 366).
                 */
            case 'j':
                snprintf(buf, MAX_BUF_SIZE, "%03d", jtm->tm_yday + 1);
                break;

                /*
                 * The hour (24-hour clock) as a decimal number (range 0 to 23);
                 * single digits are preceded by a blank.
                 * (See  also %H.)
                 */
            case 'k':
                snprintf(buf, MAX_BUF_SIZE, "%2d", jtm->tm_hour);
                break;

                /*
                 * The  hour (12-hour clock) as a decimal number
                 * (range 1 to 12);
                 * single digits are preceded by a blank.
                 * (See also %I.)
                 */
            case 'l':
                tmp = (jtm->tm_hour == 12) ? 12 : jtm->tm_hour % 12;
                snprintf(buf, MAX_BUF_SIZE, "%2d", tmp);
                break;

                /* The month as a decimal number (range 01 to 12). */
            case 'm':
                snprintf(buf, MAX_BUF_SIZE, "%02d", jtm->tm_mon + 1);
                break;

                /* The minute as a decimal number (range 00 to 59). */
            case 'M':
                snprintf(buf, MAX_BUF_SIZE, "%02d", jtm->tm_min);
                break;

                /* A newline character. */
            case 'n':
                snprintf(buf, MAX_BUF_SIZE, "\n");
                break;

                /*
                 * Either "ق.ظ" or "ب.ظ" according to the given time value.
                 * Noon is treated as "ق.ظ" and midnight as "ب.ظ".
                 */
            case 'O':
                snprintf(buf, MAX_BUF_SIZE, "%s",
                         (jtm->tm_hour >= 0 && jtm->tm_hour < 12) ?
                         "ق.ظ" : "ب.ظ");
                break;

                /*
                 * Either "AM" or "PM" according to the given time value.
                 * Noon is treated as "PM" and midnight as "AM".
                 */
            case 'p':
                snprintf(buf, MAX_BUF_SIZE, "%s",
                         (jtm->tm_hour >= 0 && jtm->tm_hour < 12) ?
                         "AM" : "PM");
                break;

                /* Like %p but in lowercase: "am" or "pm". */
            case 'P':
                snprintf(buf, MAX_BUF_SIZE, "%s",
                         (jtm->tm_hour >= 0 && jtm->tm_hour < 12) ?
                         "am" : "pm");
                break;

                /*
                 * The time in a.m. or p.m. notation.
                 * In the POSIX locale this is equivalent to %I:%M:%S %p.
                 */
            case 'r':
                snprintf(buf, MAX_BUF_SIZE, "%02d:%02d:%02d %s",
                         (jtm->tm_hour == 12) ? 12 : jtm->tm_hour % 12,
                         jtm->tm_min, jtm->tm_sec,
                         (jtm->tm_hour >= 0 && jtm->tm_hour < 12) ?
                         "AM" : "PM");
                break;

                /*
                 * The time in 24-hour notation (%H:%M).
                 * For a version including the seconds, see %T below.
                 */
            case 'R':
                snprintf(buf, MAX_BUF_SIZE, "%02d:%02d", jtm->tm_hour,
                         jtm->tm_min);
                break;

                /*
                 * The number of seconds since the Epoch
                 * 1970-01-01 00:00:00 +0000 (UTC).
                 */
            case 's':
                memcpy(&t_j, jtm, sizeof(struct jtm));
                t = jmktime(&t_j);
                snprintf(buf, MAX_BUF_SIZE, "%d", (int) t);
                break;

                /* The second as a decimal number (range 00 to 59). */
            case 'S':
                snprintf(buf, MAX_BUF_SIZE, "%02d", jtm->tm_sec);
                break;

                /* A tab character. */
            case 't':
                snprintf(buf, MAX_BUF_SIZE, "\t");
                break;

                /* The time in 24-hour notation (%H:%M:%S). */
            case 'T':
                snprintf(buf, MAX_BUF_SIZE, "%02d:%02d:%02d",
                         jtm->tm_hour, jtm->tm_min, jtm->tm_sec);
                break;

                /*
                 * The day of the week as a decimal, range 1 to 7
                 * Saturday being 1.  See also %w.
                 */
            case 'u':
                snprintf(buf, MAX_BUF_SIZE, "%d", jtm->tm_wday + 1);
                break;

                /*
                 * The week  number of the current year as a decimal number,
                 * range 00 to 53,
                 * starting with the first Saturday as the first day of week 01.
                 */
            case 'U':
                memcpy(&t_j, jtm, sizeof(struct jtm));
                t_j.tm_yday = 0;
                jalali_create_date_from_days(&t_j);
                tmp = (jtm->tm_yday + t_j.tm_wday) / 7;
                snprintf(buf, MAX_BUF_SIZE, "%02d", tmp);
                break;

                /*
                 * The day of the week as a decimal, range 0 to 6
                 * Saturday being 0.  See also %u.
                 */
            case 'w':
                snprintf(buf, MAX_BUF_SIZE, "%d", jtm->tm_wday);
                break;

                /*
                 * The preferred date representation without the time
                 * in Farsi. (utf8)
                 */
            case 'W':
                jalali_to_farsi(_la, 100, 0, " ", jtm->tm_year);
                jalali_to_farsi(_l1, 10, 2, "۰", jtm->tm_mon+1);
                jalali_to_farsi(_l2, 10, 2, "۰", jtm->tm_mday);

                snprintf(buf, MAX_BUF_SIZE, "%s/%s/%s", _la,
                         _l1, _l2);
                break;

                /* The preferred date representation without the time. */
            case 'x':
                snprintf(buf, MAX_BUF_SIZE, "%02d/%02d/%d",
                         jtm->tm_mday, jtm->tm_mon+1, jtm->tm_year);
                break;

                /*
                 * The preferred time representation in Farsi. (utf8)
                 */
            case 'X':
                jalali_to_farsi(_l1, 10, 2, "۰", jtm->tm_hour);
                jalali_to_farsi(_l2, 10, 2, "۰", jtm->tm_min);
                jalali_to_farsi(_l3, 10, 2, "۰", jtm->tm_sec);

                snprintf(buf, MAX_BUF_SIZE, "%s:%s:%s",
                         _l1, _l2, _l3);

                break;

                /*
                 * The year as a decimal number without a century
                 * (range 00 to 99).
                 */
            case 'y':
                tmp = ((jtm->tm_year) % 1000) % 100;
                snprintf(buf, MAX_BUF_SIZE, "%02d", tmp);
                break;

                /* The year as a decimal number including the century. */
            case 'Y':
                snprintf(buf, MAX_BUF_SIZE, "%d", jtm->tm_year);
                break;

                /*
                 * The +hhmm or -hhmm numeric timezone
                 * (that is, the hour and minute offset from UTC).
                 */
            case 'z':
                tmp = ((int)jtm->tm_gmtoff / J_HOUR_LENGTH_IN_SECONDS);
                tmp1 = ((int)jtm->tm_gmtoff % J_HOUR_LENGTH_IN_SECONDS) /
                    J_MINUTE_LENGTH_IN_SECONDS;
                snprintf(buf, MAX_BUF_SIZE, "%s%02d%02d",
                         (tmp >= 0) ? "+" : "-", abs(tmp), abs(tmp1));
                break;

                /* The timezone or name or abbreviation. */
            case 'Z':
                snprintf(buf, MAX_BUF_SIZE, "%s", jtm->tm_zone);
                break;

                /* A literal '%' character. */
            case '%':
                snprintf(buf, MAX_BUF_SIZE, "%s", "%");
                break;

                /* Non of the above. Ignoring modifier. */
            default:
                break;
            }

            b_n = strlen(buf);
            for (j=0; j<b_n; j++) {
                if (rb > (int) (max - 2)) {
                    s[max-1] =  '\0';
                    return (max-1);
                }

                s[rb] = buf[j];
                rb++;
            }
            i++;
            _la[0] = 0;
            _lb[0] = 0;
            _l1[0] = 0;
            _l2[0] = 0;
            _l3[0] = 0;
        }
    }
    s[rb] = '\0';

    return rb;
}

char* jstrptime(const char* s, const char* format, struct jtm* jtm)
{
    char buf[MAX_BUF_SIZE];
    char delim[MAX_BUF_SIZE];

    char* pos_n;
    char* pos_c;
    char* pos_e;

    char** ptr;

    size_t diff = 0, diff1 = 0;
    int s_s, fmt_s, tmp;
    int i, j, k, f, c = 0;
    char fd;

    struct jtm _j;
    time_t t;

    s_s = strlen(s);
    fmt_s = strlen(format);

    /*
     * Traversing format string for the matching characters in input string.
     * Each time an identical character is found, we simply pass to the
     * next one. In the event of finding a % (format specifier) we find the
     * matching argument by means of finding the delimiter between the found
     * format specifier and the next. If there was no format specifiers after
     * the current one, we consider the remaining set of characters as
     * delimiter.
     */

    for (i=0, j=0; i < s_s && j < fmt_s; ) {
        buf[0] = 0;
        delim[0] = 0;

        /* Identical character in format and string, skipping. */
        if (s[i] == format[j]) {
            i++;
            j++;
            continue;
        }

        /* Malformed string or format. */
        if (format[j] != '%') {
            return NULL;
        }

        if (j+2 >= fmt_s) {
            diff1 = s_s;
        } else {
            pos_c = (char*) &format[j+2];
            pos_n = strchr(pos_c, '%');
            if (!pos_n) {
                diff = 1;
                pos_n = (char*) &format[fmt_s-1];
            } else {
                diff = (pos_n - pos_c);
            }

            memcpy(delim, pos_c, diff);
            delim[diff] = 0;
            pos_e = strstr(&s[i], delim);

            /* Delimiter not found in string. */
            if (!pos_e) {
                return NULL;
            }

            diff1 = (pos_e - &s[i]);
        }

        fd = format[j+1];
        memcpy(buf, &s[i], diff1);
        buf[diff1] = 0;
        switch (fd) {
            /* The abbreviated or full weekday name. */
        case 'a':
        case 'A':
            ptr = (fd == 'a') ? (char**) jalali_days_3 : (char**) jalali_days;
            f = 0;

            for (k=0; k<J_WEEK_LENGTH; k++) {
                if (!strcasecmp(buf, ptr[k])) {
                    jtm->tm_wday = k;
                    f = 1;
                }
            }

            if (!f)
                return (char*) &s[i];

            break;

            /* The abbreviated or full month name. */
        case 'b':
        case 'B':
            ptr = (fd == 'b') ? (char**) jalali_months_3 :
            (char**) jalali_months;
            f = 0;

            for (k=0; k<J_YEAR_LENGTH_IN_MONTHS; k++) {
                if (!strcasecmp(buf, ptr[k])) {
                    jtm->tm_mon = k;
                    f = 1;
                }
            }

            if (!f)
                return (char*) &s[i];

            break;

            /* The day of the month as a decimal number (range 01 to 31). */
        case 'd':
        case 'e':
            jtm->tm_mday = atoi(buf);
            break;

            /*
             * The hour as a decimal number using a 24-hour clock
             * (range 00 to 23).
             */
        case 'H':
            jtm->tm_hour = atoi(buf);
            break;

            /* The day of the year as a decimal number (range 001 to 366). */
        case 'j':
            jtm->tm_yday = atoi(buf) - 1;
            break;

            /* The month as a decimal number (range 01 to 12). */
        case 'm':
            jtm->tm_mon = atoi(buf) -1;
            break;

            /* The minute as a decimal number (range 00 to 59). */
        case 'M':
            jtm->tm_min = atoi(buf);
            break;

            /* Seconds since epoch. (1970/1/1) */
        case 's':
            t = (time_t) atol(buf);
            jlocaltime_r(&t, &_j);
            memcpy(jtm, &_j, sizeof(struct jtm));
            break;

            /* The second as a decimal number (range 00 to 59). */
        case 'S':
            jtm->tm_sec = atoi(buf);
            break;

            /*
             * The year as a decimal number without a century
             * (range 00 to 99).
             */
        case 'y':
            tmp = atoi(buf);
            if (tmp >= 19 && tmp < 100)
                jtm->tm_year = 1300 + tmp;
            else
                jtm->tm_year = 1400 + tmp;
            break;

            /* The year as a decimal number including the century. */
        case 'Y':
            jtm->tm_year = atoi(buf);
            break;

            /* The abbreviated or full weekday name. (Farsi) */
        case 'q':
        case 'h':
            ptr = (fd == 'h') ? (char**) jalali_days_3_fa :
            (char**) jalali_days_fa;
            f = 0;

            for (k=0; k<J_WEEK_LENGTH; k++) {
                if (!strcasecmp(buf, ptr[k])) {
                    jtm->tm_wday = k;
                    f = 1;
                }
            }

            if (!f)
                return (char*) &s[i];

            break;

        default:
            break;
        }


        c++;
        j += diff + 2;
        i += diff1 + diff;
    }

    return (char*) &s[s_s];
}

char* jasctime_r(const struct jtm* jtm, char* buf)
{
    if (!jtm || !buf)
        return 0;

    in_jasctime(jtm, buf);

    return buf;
}

struct jtm* jlocaltime_r(const time_t* timep, struct jtm* result)
{
    if (!timep || !result)
        return 0;

    in_jlocaltime(timep, result);

    return result;
}

struct jtm* jgmtime_r(const time_t* timep, struct jtm* result)
{
    if (!timep || !result)
        return 0;

    in_jgmtime(timep, result);

    return result;
}

char* jctime_r(const time_t* timep, char* buf)
{
    if (!timep || !buf)
        return 0;

    in_jctime(timep, buf);

    return buf;
}

/*
 * @Utils
 * Utility functions for internal use.
 * jalali_to_farsi() converts an integer's digits to Arabic-Indic
 * padding works just like printf() field width.
 */
int jalali_to_farsi(char* buf,
                    size_t n,
                    int padding,
                    char* pad,
                    int d)
{
    char _buf[100] = {0};
    int i=0, j=0;
    int p = 0;
    int c = 0;
    int cw = (pad[0] < 0) ? 2 : 1;

    for (i=d; i!=0; c++,
             _buf[p] = farsi_digits[i%10 > 0 ? i%10 : -(i%10)][1],
             _buf[p+1] = farsi_digits[i%10 > 0 ? i%10 : -(i%10)][0],
             i/=10,
             p+=2);

    if (d < 0) {
        _buf[p] = '-';
        c++;
        p++;
    }

    _buf[p]= 0;
    buf[0] = 0;
    i=0;

    for (i=0; (i<(padding - c)) && (i*cw < (int)(n-1));
         strcat(buf, pad), i++);
    buf[i*cw] = 0;

    for (j=0, i*=cw; (j<p) && (i < (int)(n-1));
         buf[i] = _buf[p - j - 1], i++, j++);
    buf[i] = 0;

    return i;
}

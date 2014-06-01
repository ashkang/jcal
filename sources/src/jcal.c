/*
 * jcal.c - Unix cal-like interface to libjalali.
 * Copyright (C) 2006, 2007, 2009, 2010, 2011 Ashkan Ghassemi.
 *
 * This file is part of jcal.
 *
 * jcal is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * jcal is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.      See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with jcal.      If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "termcap.h"
#include "jcal.h"
#include "../libjalali/jalali.h"
#include "../libjalali/jtime.h"

extern const char* jalali_months[];
extern const char* jalali_days[];
extern const char* jalali_days_fa[];
extern const char* jalali_days_3[];
extern const char* jalali_days_3_fa[];
extern const char* jalali_days_2[];
extern const char* jalali_days_2_fa[];
extern const char* fa_jalali_months[];
extern const char* fa_jalali_days_3[];
extern const char* fa_jalali_days_2[];

extern const int jalali_month_len[];

extern char* optarg;

/*
 * @Create, set, destroy and show set of cal_matrix functions.
 * To avoid using cursor movement escape sequences (ANSI, vt100, etc.)
 * we create a matrix of calendar days to form a standard sequence of
 * numbers to be printed verbatim on screen. Each matrix can have unlimited
 * number of monthly calendars thus the current version of jcal is simple and
 * does not require curses or similar libraries and is effectively independent
 * of termcap.
 * To disable standard terminal escape sequences for colors completely,
 * compile with NO_COLOR.
 */

/*
 * Sets the matrix according to a given jalali date and prefix.
 */

void set_cal_matrix(struct cal_layout* l,
                    struct jtm* ct,
                    struct cal_matrix* mat,
                    int prefix)
{
    struct jtm mb;
    struct jtm lt;

    time_t t;

    int diff;
    int _prefix = prefix * (7 + l->margin);
    int m, c;
    int i, j;

    time(&t);
    jlocaltime_r(&t, &lt);

    memcpy(&mb, ct, sizeof(struct jtm));

    diff = (mb.tm_mday - 1) % 7;
    mb.tm_wday = (mb.tm_wday - diff) % 7;
    if (mb.tm_wday < 0)
        mb.tm_wday += 7;

    mb.tm_mday = 1;
    jalali_create_days_from_date(&mb);

    m = (jalali_is_jleap(mb.tm_year) && mb.tm_mon == 11) ? 30 :
        jalali_month_len[mb.tm_mon];

    j = mb.tm_wday;
    for (i=0,c=0; i<=mat->height && c < m; i++) {
        for (; j<=6 && c < m; j++) {
            if ((mb.tm_year == lt.tm_year) && (mb.tm_mon == lt.tm_mon)
                && (mb.tm_mday + c == lt.tm_mday)) {
                mat->m[i][j+_prefix] = (l->julian) ? 1000 + mb.tm_yday + c + 1 :
                    1000 + mb.tm_mday + c;
            } else {
                mat->m[i][j+_prefix] = (l->julian) ? mb.tm_yday + c + 1 :
                    mb.tm_mday + c;
            }
            c++;
        }
        j = 0;
    }
}


/*
 * Returns 0, if the given column is located within margins between calendars,
 * 1 otherwise.
 */

int is_in_margin(struct cal_layout* l,
                 struct cal_matrix* mat,
                 int c)
{
    int i;

    if (mat->n == 1) {
        if (c < 6)
            return 0;
        else if (c == 6)
            return 2;
        else
            return 1;
    }

    for (i=0; i<=mat->n - 1; i++) {
        if (c < (i * (7 + l->margin)) + 7 - 1) {
            return 0;
        }
        else if (c == (i * (7 + l->margin) + 7) - 1) {
            return 2;
        }
        else if ((c >= (i * (7 + l->margin) + 7)) &&
                 (c < ((i+1) * (7 + l->margin)))) {
            return 1;
        }
    }
    return 0;
}


/*
 * Displays the calendar matrix over standard screen.
 * @Should not be used directly. (See display_cal())
 */

void show_cal_matrix(struct cal_layout* l, struct cal_matrix* mat)
{
    int i, j, m;
    char buf[100];

    for (i=0; i<mat->height; i++) {
        for (j=0; j<mat->width; j++) {
            m = is_in_margin(l, mat, j);
            if (m == 1) {
                printf(" ");
            }
            else if (m == 0) {
                if (mat->m[i][j] == 0)
                    printf((l->julian) ? "    " : "   ");
                else if (mat->m[i][j] > 1000 && l->color) {
                    if (l->farsi) {
                        jalali_to_farsi(buf, 100, 2+l->julian,
                                 " ", mat->m[i][j] - 1000);
                    } else {
                        snprintf(buf, 100, "%*d",
                                 2+l->julian, mat->m[i][j] - 1000);
                    }

                    printf("%s%s%s ",
                           TERM_BLACK_ON_WHITE,
                           buf, TERM_RESET);

                } else if (mat->m[i][j] > 1000 && !l->color) {
                    if (l->farsi) {
                        jalali_to_farsi(buf, 100, 2+l->julian,
                                 " ", mat->m[i][j] - 1000);
                    } else {
                        snprintf(buf, 100, "%*d",
                                 2+l->julian, mat->m[i][j] - 1000);
                    }

                    printf("%s ", buf);

                }
                else {
                    if (l->farsi) {
                        jalali_to_farsi(buf, 100, 2+l->julian,
                                        " ", mat->m[i][j]);
                    } else {
                        snprintf(buf, 100, "%*d",
                                 2+l->julian, mat->m[i][j]);
                    }

                    printf("%s ", buf);
                }
            }
            else {
                if (mat->m[i][j] == 0)
                    printf((l->julian) ? "   " : "  ");
                else if (mat->m[i][j] > 1000 && l->color) {
                    if (l->farsi) {
                        jalali_to_farsi(buf, 100, 2+l->julian,
                                        " ", mat->m[i][j] - 1000);
                    } else {
                        snprintf(buf, 100, "%*d",
                                 2+l->julian, mat->m[i][j] - 1000);
                    }

                    printf("%s%s%s",
                           TERM_RED_ON_WHITE,
                           buf, TERM_RESET);

                } else if ((mat->m[i][j] > 1000) && (!l->color)) {
                    if (l->farsi) {
                        jalali_to_farsi(buf, 100, 2+l->julian,
                                        " ", mat->m[i][j] - 1000);
                    } else {
                        snprintf(buf, 100, "%*d",
                                 2+l->julian, mat->m[i][j] - 1000);
                    }

                    printf("%s", buf);
                } else if (l->color) {
                    if (l->farsi) {
                        jalali_to_farsi(buf, 100, 2+l->julian,
                                        " ", mat->m[i][j]);
                    } else {
                        snprintf(buf, 100, "%*d",
                                 2+l->julian, mat->m[i][j]);
                    }

                    printf("%s%s%s",
                           TERM_RED,
                           buf,
                           TERM_RESET);

                }
                else {
                    if (l->farsi) {
                        jalali_to_farsi(buf, 100, 2+l->julian,
                                        " ", mat->m[i][j]);
                    } else {
                        snprintf(buf, 100, "%*d",
                                 2+l->julian, mat->m[i][j]);
                    }

                    printf("%s", buf);
                }
            }
            buf[0] = 0;
        }
        printf("\n");
    }
}

/*
 * Allocates memory for our calendar matrix and sets it's width
 * and height according to number of calendars.
 */

void create_cal_matrix(struct cal_layout* l, struct cal_matrix* mat)
{
    mat->width = (mat->n * 7) + ((mat->n - 1) * l->margin) ;
    mat->height = 6;
    mat->m = malloc(mat->height * sizeof(int*));

    int i;

    for (i=0; i<mat->height; i++) {
        mat->m[i] = malloc(mat->width * sizeof(int));
        memset(mat->m[i], 0, mat->width * sizeof(int));
    }
}


/*
 * Frees the allocated calendar memory.
 */

void destroy_cal_matrix(struct cal_matrix* mat)
{
    int i;

    for (i=0; i<mat->height; i++) {
        free(mat->m[i]);
    }

    free(mat->m);
}


/*
 * Displays calendar, including cal title and week days.
 */

void show_cal(struct cal_layout* l,
              struct cal_matrix* m,
              struct jtm** _j)
{
    char** ptr_d;

    int i, k;
    int cw = (l->farsi) ? 2 : 1;
    int cal_width = (l->julian) ? 7 * 3 + 6 : 7 * 2 + 6;

    char cal_t[3][MAX_BUF_SIZE];
    char cal_y[3][100];
    int cal_tw[3];
    char buf[100];

    if (l->farsi)
        ptr_d = (l->julian) ? (char**) fa_jalali_days_3 :
            (char**) fa_jalali_days_2;
    else if (l->english)
        ptr_d = (l->julian) ? (char**) jalali_days_3 : (char**) jalali_days_2;
    else
        ptr_d = (l->julian) ? (char**) jalali_days_3_fa :
            (char**) jalali_days_2_fa;


    for (i=0; i<m->n; i++) {
        if (l->farsi) {
            jalali_to_farsi(buf, 100, 0, " ",
                     _j[i]->tm_year + ((l->pahlavi) ?
                                       PAHLAVI_ISLAMIC_DIFF : 0));
        } else {
            snprintf(buf, 100, "%d",
                     _j[i]->tm_year + ((l->pahlavi) ?
                                       PAHLAVI_ISLAMIC_DIFF : 0));
        }
        snprintf(cal_y[i], 100, "%s%s",
                 buf,
                 (l->pahlavi) ? ((l->farsi) ? " په" : "(pa)") : "");
        buf[0] = 0;
    }

    for (i=0; i<m->n; i++) {
        snprintf(cal_t[i], MAX_BUF_SIZE, "%s %s", (l->farsi) ?
                 fa_jalali_months[_j[i]->tm_mon] :
                 jalali_months[_j[i]->tm_mon],
                 (l->syear) ? cal_y[i] : "");
        cal_tw[i] = (cal_width - (strlen(cal_t[i]) / cw)) / 2;
    }

    for (i=0; i<m->n; i++) {
        for (k=0; k<cal_tw[i]; k++) {
            printf(" ");
        }

        printf("%s%s%s", TERM_WHITE, cal_t[i], TERM_RESET);

        for (k=0;
             k<(cal_width - cal_tw[i] -
                (strlen(cal_t[i]) / cw + ((cw > 1) ? 1 : 0)));
             k++) {
            printf(" ");
        }

        if (i != m->n-1) {
            for (k=0; k<l->margin; k++) {
                printf(" ");
            }
        }
    }

    printf("\n");

    for (i=0; i<m->n; i++) {
        for (k=0; k<6; k++) {
            printf("%s%s%s ", TERM_WHITE, ptr_d[k], TERM_RESET);
        }

        if (l->color)
            printf("%s%s%s", TERM_RED, ptr_d[6], TERM_RESET);
        else
            printf("%s%s%s", TERM_WHITE, ptr_d[6], TERM_RESET);

        if (i != m->n-1) {
            for (k=0; k<l->margin; k++) {
                printf(" ");
            }
        }
    }

    printf("\n");

    create_cal_matrix(l, m);

    for (i=0; i<m->n; i++) {
        set_cal_matrix(l, _j[i], m, i);
    }

    show_cal_matrix(l, m);
    destroy_cal_matrix(m);
}


/*
 * Displays previous, current and next month's calendar on standard
 * screen according to a given jalali date.
 */

void show_3(struct cal_layout* l, struct jtm* j)
{
    struct jtm** _j;
    struct cal_matrix m;

    int diff_p;
    int diff_c;
    int diff_n;

    int i;

    m.n = 3;

    _j = malloc(m.n * sizeof(struct jtm*));

    for (i=0; i<m.n; i++) {
        _j[i] = malloc(sizeof(struct jtm));
    }

    memcpy(_j[1], j, sizeof(struct jtm));

    diff_c = jalali_get_diff(_j[1]);
    diff_p = diff_c - (_j[1]->tm_mday + 1);
    diff_n = (jalali_is_jleap(_j[1]->tm_year) && _j[1]->tm_mon == 11) ?
        diff_c + (30 - _j[1]->tm_mday + 1) :
        diff_c + (jalali_month_len[_j[1]->tm_mon] - _j[1]->tm_mday + 1);

    jalali_get_date(diff_p, _j[0]);
    jalali_get_date(diff_n, _j[2]);
    show_cal(l, &m, _j);

    for (i=0; i<3; i++) {
        free(_j[i]);
    }

    free(_j);
}


/*
 * Displays a calendar on standard screen according to a given jalali date.
 */

void show_1(struct cal_layout* l, struct jtm* j)
{
    struct jtm** _j;
    struct cal_matrix m;

    l->syear = 1;

    _j = malloc(sizeof(struct jtm*));
    _j[0] = malloc(sizeof(struct jtm));

    memcpy(_j[0], j, sizeof(struct jtm));
    m.n = 1;
    show_cal(l, &m, _j);

    free(_j[0]);
    free(_j);
}


/*
 * Displays a whole year calendar on standard screen according
 * to a given jalali date.
 */

void show_year(struct cal_layout* l, struct jtm* j)
{
    struct jtm _j[4] = {{0}};

    char title[100];
    char buf[100];

    int cal_width = (((l->julian) ? (3 * 7 + 6) : (2 * 7 + 6)) * 3) +
        (2 * l->margin);

    int cal_tw;
    int i;

    if (l->farsi) {
        jalali_to_farsi(buf, 100, 0, " ", j->tm_year + ((l->pahlavi) ?
                                                 PAHLAVI_ISLAMIC_DIFF : 0));
    } else {
        snprintf(buf, 100, "%d", j->tm_year + ((l->pahlavi) ?
                                                 PAHLAVI_ISLAMIC_DIFF : 0));
    }

    snprintf(title, 100, "%s%s", buf,
             (l->pahlavi) ? (l->farsi ? " پهلوی" : " (Pahlavi)") : "");

    cal_tw = (cal_width - strlen(title)) / 2;

    for (i=0; i<cal_tw; i++) {
        printf(" ");
    }
    printf("%s\n\n", title);

    l->syear = 0;

    _j[0].tm_year = j->tm_year;
    _j[0].tm_mon = 1;
    _j[0].tm_mday = 1;
    jalali_update(&_j[0]);

    _j[1].tm_year = j->tm_year;
    _j[1].tm_mon = 4;
    _j[1].tm_mday = 1;
    jalali_update(&_j[1]);

    _j[2].tm_year = j->tm_year;
    _j[2].tm_mon = 7;
    _j[2].tm_mday = 1;
    jalali_update(&_j[2]);

    _j[3].tm_year = j->tm_year;
    _j[3].tm_mon = 10;
    _j[3].tm_mday = 1;
    jalali_update(&_j[3]);

    show_3(l, &_j[0]);
    show_3(l, &_j[1]);
    show_3(l, &_j[2]);
    show_3(l, &_j[3]);
}

int main(int argc, char** argv)
{
    struct cal_layout l;
    struct jtm j = {0};
    time_t t;

    int opt;
    int i, c = 0;
    int def_date[3];
    void (*show) (struct cal_layout*, struct jtm*);

    show = &show_1;
    time(&t);
    jlocaltime_r(&t, &j);

    /* Default values for display date */
    def_date[0] = 1;
    def_date[1] = 1;
    def_date[2] = j.tm_year;

    l.color = 1;
    l.pahlavi = 0;
    l.farsi = 0;
    l.julian = 0;
    l.english = 0;
    l.margin = 3;

    /* Parsing date values. (YYYY MM DD) */
    for (i=1; i<argc && c<3; i++) {
        opt = atoi(argv[i]);
        if (opt > 0) {
            def_date[2 - c] = opt;
            c++;
        }
    }

    if (c > 0) {
        if (c == 1)
            show = &show_year;
        else
            show = &show_1;

        if (def_date[0] > 31 || def_date[0] < 1) {
            printf("%s: illegal day value: use 1-31\n", argv[0]);
            exit(EXIT_FAILURE);
        }

        if (def_date[1] > 12 || def_date[1] < 1) {
            printf("%s: illegal month value: use 1-12\n", argv[0]);
            exit(EXIT_FAILURE);
        }

        j.tm_year = def_date[2];
        j.tm_mon = def_date[1] - 1;
        j.tm_mday = def_date[0];
        jalali_update(&j);
    }

    /* Parsing short options. */
    while ((opt = getopt(argc, argv, JCAL_VALID_ARGS)) != -1) {
        switch (opt) {
            /* Using Pahlavi instead of Islamic Epoch. */
        case 'P':
            l.pahlavi = 1;
            break;

        case 'p':
            l.farsi = 1;
            l.english = 0;
            break;

            /* Displays Julian days (Day of year) instead of day of month.  */
        case 'j':
            l.julian = 1;
            break;

            /* Displays English names of weekdays. */
        case 'e':
            l.english = 1;
            break;

            /* Display one-month calendar for a given date. */
        case '1':
            show = &show_1;
            break;

            /*
             * Display previous, current and next month's calendar
             * for the given date.
             */
        case '3':
            l.syear = 1;
            show = &show_3;
            break;
            /* Whole-year calendar for a given date. */
        case 'y':
            show = &show_year;
            break;

            /* No-color. */
        case 'N':
            l.color = 0;
            break;

            /* Version information. */
        case 'V':
            printf("jcal version %s (libjalali-%s)\n",
                   JCAL_VERSION, LIBJALALI_VERSION);
            printf("Written by Ashkan Ghassemi.\n");
            exit(EXIT_SUCCESS);
            break;

            /* Unknown parameter. */
        default:
            printf("usage: jcal [-13jypPV] [year [month[day]]]\n");
            exit(EXIT_FAILURE);
        }
    }

    show(&l, &j);
    exit(EXIT_SUCCESS);
}

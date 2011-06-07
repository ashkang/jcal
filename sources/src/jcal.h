/*
 * jcal.h - Unix cal-like interface to libjalali.
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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	 See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with jcal.	 If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef JCAL_H
#define JCAL_H

#include "../libjalali/jalali.h"
#include "../libjalali/jtime.h"

#define JCAL_VALID_ARGS "13jyVNePp"
#define PAHLAVI_ISLAMIC_DIFF 1180

#define JCAL_VERSION "0.4"

struct cal_layout {
	int color;		/* If enabled, drawer tries to colorize output. */
	int julian;	    /* Displays julian days (1-366) instead of month days. */
	int pahlavi;	/* jcal uses Pahlavi instead of Islamic epoch. */
	int english;	/* Use Farsi transliteration for weekday names. */
	int farsi;      /* Use Farsi utf8 names and numbers. */
	int margin;	    /* Marginal space between two cals. */
	int syear;		/* If enabled, jcal shows year above cals. */
};

struct cal_matrix {
	int n;		/* Calendar numbers within matrix. */
	int** m;	/* Calendar matrix. */
	int width;	/* Matrix width. */
	int height; /* Matrix height. */
};

void set_cal_matrix(struct cal_layout* l, struct jtm* ct,
					struct cal_matrix* mat, int prefix);

int is_in_margin(struct cal_layout* l, struct cal_matrix* mat, int c);

void show_cal_matrix(struct cal_layout* l, struct cal_matrix* mat);

void create_cal_matrix(struct cal_layout* l, struct cal_matrix* mat);

void destroy_cal_matrix(struct cal_matrix* mat);

void show_cal(struct cal_layout* l, struct cal_matrix* m, struct jtm** _j);

void show_3(struct cal_layout* l, struct jtm* j);

void show_1(struct cal_layout* l, struct jtm* j);

void show_year(struct cal_layout* l, struct jtm* j);


#endif /* JCAL_H */

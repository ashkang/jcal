/*
 * jalali.c - Tools for manipulating Jalali representation of Iranian calendar
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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	 See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with libjalali.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdio.h>
#include <limits.h>
#include <math.h>
#include <time.h>
#include "jalali.h"
#include "jconfig.h"

const int cycle_patterns[] = { J_PT0, J_PT1, J_PT2, J_PT3, INT_MAX };
const int leaps[] = { J_L0, J_L1, J_L2, J_L3, INT_MAX };

const int jalali_month_len[] = { 31, 31, 31, 31, 31, 31, 30, 30, 30, 30,
								 30, 29 };

extern char* tzname[2];

/*
 * Jalali leap year indication function. The algorithm used here
 * is loosely based on the famous recurring 2820 years length period. This
 * period is then divided into 88 cycles, each following a 29, 33, 33, 33
 * years length pattern with the exception for the last being 37 years long.
 * In every of these 29, 33 or 37 years long periods starting with year 0,
 * leap years are multiples of four except for year 0 in each period.
 * The current 2820 year period started in the year AP 475 (AD 1096).
 */

int
jalali_is_jleap(int year)
{
	int pr = year;

	/* Shifting ``year'' with 2820 year period epoch. */
	pr -= JALALI_LEAP_BASE;

	pr %= JALALI_LEAP_PERIOD;

	/*
	 * According to C99 standards, modulo operator's result has the same sign
	 * as dividend. Since what we require to process has to be in range
	 * 0-2819, we have to shift the remainder to be positive if dividend is
	 * negative.
	 */
	if (pr < 0) {
		pr += JALALI_LEAP_PERIOD;
	}

	/*
	 * Every cycle consists of one 29 year period and three identical 33 year
	 * periods forming a 128 years length cycle. An exception applies to the
	 * last cycle being 132 years instead and it's last 33 years long partition
	 * will be extended for an extra 4 years thus becoming 37 years long.
	 * JALALI_LAST_CYCLE_START literally marks the beginning of this last
	 * cycle.
	 */

	pr = (pr > JALALI_LAST_CYCLE_START) ?
		(pr - JALALI_LAST_CYCLE_START) : pr % JALALI_NORMAL_CYCLE_LENGTH;

	/*
	 * Classifying year in a cycle. Assigning to one of the four partitions.
	 */
	int i;
	for (i=0; i<J_LI; i++)
	{
		if ((pr >= cycle_patterns[i]) && (pr < cycle_patterns[i+1]))
		{
			pr -= cycle_patterns[i];
			/* Handling year-0 exception */
			if (!pr) /* pr is zero */
				return 0;
			/*
			 * If year is a multiple of four then it's leap,
			 * ordinary otherwise.
			 */
			else
				return !(pr % J_LI);
		}
	}

	/*
	 * Our code flow better not reach this fail-safe
	 * return statement and I really mean it.
	 */
	return 0;
}

/*
 * Creates absolute values for day, hour, minute and seconds from time_t.
 * Values are signed integers.
 */
void
jalali_create_time_from_secs(time_t t, struct ab_jtm* d)
{
	d->ab_days = (t / J_DAY_LENGTH_IN_SECONDS);
	t %= J_DAY_LENGTH_IN_SECONDS;
	d->ab_hour = t / J_HOUR_LENGTH_IN_SECONDS;
	t %= J_HOUR_LENGTH_IN_SECONDS;
	d->ab_min = t / J_MINUTE_LENGTH_IN_SECONDS;
	d->ab_sec = t % J_MINUTE_LENGTH_IN_SECONDS;
}

/*
 * Creates a timestamp from day, hour, minute and seconds.
 * Values are signed integers.
 */
time_t
jalali_create_secs_from_time(const struct ab_jtm* d)
{
	return (d->ab_days * J_DAY_LENGTH_IN_SECONDS +
			d->ab_hour * J_HOUR_LENGTH_IN_SECONDS +
			d->ab_min * J_MINUTE_LENGTH_IN_SECONDS +
			d->ab_sec);
}

/*
 * Month and day of year calculation for a desired day of year.
 * Alters only tm_mday and tm_mon.
 * Zero on success, -1 on failure.
 */
int
jalali_create_date_from_days(struct jtm* j)
{
	int p = j->tm_yday;
	if (p > 365 || p < 0)
		return -1;

	p++;
	int i;

	/* Traversing all twelve months, ranging from 0 to 11 */
	for (i=0; i<11; i++) {
		if (p > jalali_month_len[i])
			p -= jalali_month_len[i];
		else
			break;
	}

	j->tm_mday = p;
	j->tm_mon = i;

	return 0;
}

/*
 * Calculate day of year (0-365) based on month and day.
 */
int
jalali_create_days_from_date(struct jtm* j)
{
	int p = 0;
	int i;
	if (j->tm_mon < 0 || j->tm_mon > 11)
		return -1;

	if (j->tm_mday < 1 || j->tm_mday > 31)
		return -1;

	for (i=0; i<j->tm_mon; i++) {
		p+= jalali_month_len[i];
	}

	p+= j->tm_mday - 1;
	j->tm_yday = p;

	return 0;
}

/*
 * Get useful information on a desired jalali year, including:
 * 1. Leap status. -lf
 * 2. Year position in grand leap cycle, passed and remaining years. -p, -r
 * 3. Passed and remaining leap years in grand leap cycle. -pl, -rl
 * 4. Absolute passed leap years since grand leap cycle epoch (AP 475). -apl
 */
void
jalali_get_jyear_info(struct jyinfo* year)
{
	int y = year->y;
	year->lf = jalali_is_jleap(year->y);
	int i;
	int d = (year->y >= JALALI_LEAP_BASE) ? 1 : -1;
	int c = 0;

	for (i=JALALI_LEAP_BASE; ; i+=d) {
		if (jalali_is_jleap(i)) {
			c++;
		}

		if (i == year->y)
			break;
	}

	year->apl = c * d;
	year->pl = (d > 0) ? c % JALALI_TOTAL_LEAPS_IN_PERIOD :
		JALALI_TOTAL_LEAPS_IN_PERIOD - (c % JALALI_TOTAL_LEAPS_IN_PERIOD);
	year->rl = JALALI_TOTAL_LEAPS_IN_PERIOD - year->pl;

	y-= JALALI_LEAP_BASE;
	y%= JALALI_LEAP_PERIOD;
	if (y < 0)
		y+= JALALI_LEAP_PERIOD;

	year->p = y;
	year->r = JALALI_LEAP_PERIOD - y - 1;

	return ;
}

/*
 * Calculates date (Jalali) based on difference factor from UTC Epoch by days.
 * 0 means 1 January 1970 (11 Dey 1348).
 */
void
jalali_get_date(int p, struct jtm* j)
{
	time_t t;
	struct tm lt;
	int wd = (p + J_UTC_EPOCH_WDAY) % J_WEEK_LENGTH;

	if (wd < 0) {
		j->tm_wday = wd + J_WEEK_LENGTH;
	} else {
		j->tm_wday = wd;
	}

	int y = J_UTC_EPOCH_YEAR, f=0;
	p += J_UTC_EPOCH_DIFF;
	int d;

	while (1) {
		d = (p >= 0 ) ? 1 : -1;
		f = jalali_is_jleap(((d > 0) ? y : y-1)) ?
			JALALI_LEAP_YEAR_LENGTH_IN_DAYS:
			JALALI_NORMAL_YEAR_LENGTH_IN_DAYS;

		if ((0 <= p) && (p < f))
			break;

		p-= (d * f);
		y+= d;
	}

	j->tm_year = y;
	j->tm_yday = p;

	jalali_create_date_from_days(j);
	tzset();
	t = p * J_DAY_LENGTH_IN_SECONDS;
	localtime_r(&t, &lt);
	j->tm_gmtoff = lt.tm_gmtoff;
	j->tm_isdst = lt.tm_isdst;
	j->tm_zone = tzname[lt.tm_isdst];
}

/*
 * Calculates UTC epoch difference of a desired date by measure of days.
 */
int
jalali_get_diff(const struct jtm* j)
{
	int p = 0;
	int i;
	int s, sd;
	int e, ed;
	int f = 1;

	if (j->tm_yday > 365 || j->tm_yday < 0)
		return -1;

	if (j->tm_year == J_UTC_EPOCH_YEAR) {
		p = j->tm_yday - J_UTC_EPOCH_DIFF;
		return p;
	}
	else if (j->tm_year > J_UTC_EPOCH_YEAR) {
		s = J_UTC_EPOCH_YEAR + 1;
		sd = J_UTC_EPOCH_DIFF;
		e = j->tm_year - 1;
		ed = j->tm_yday + 1;
	} else {
		f = -1;
		s = j->tm_year + 1;
		sd = j->tm_yday;
		e = J_UTC_EPOCH_YEAR - 1;
		ed = J_UTC_EPOCH_DIFF + 1;
	}

	for (i=s; i<=e; i++) {
		p+= jalali_is_jleap(i) ? JALALI_LEAP_YEAR_LENGTH_IN_DAYS :
			JALALI_NORMAL_YEAR_LENGTH_IN_DAYS;
	}

	int r = jalali_is_jleap(s) ? JALALI_LEAP_YEAR_LENGTH_IN_DAYS - sd - 1 :
		JALALI_NORMAL_YEAR_LENGTH_IN_DAYS - sd - 1;

	p += r + ed;
	p*= f;
	return p;
}

/*
 * Updates a jalali date struct fields based on tm_year, tm_mon and tm_mday
 */
void
jalali_update(struct jtm* jtm) {
	int d;

	jalali_create_days_from_date(jtm);
	d = jalali_get_diff(jtm);
	jalali_get_date(d, jtm);
}

/*
 * Displays a jalali date struct fields.
 * should be used for debugging purposes only.
 */
void
jalali_show_time(const struct jtm* j) {
	printf("%d/%02d/%02d (%02d:%02d:%02d) [%d]",
		   j->tm_year, j->tm_mon + 1, j->tm_mday, j->tm_hour, j->tm_min,
		   j->tm_sec, j->tm_wday);
	printf(" yday: %d, dst: %d, off: %ld, zone: %s\n",
		   j->tm_yday, j->tm_isdst, j->tm_gmtoff,
		   j->tm_zone);
}

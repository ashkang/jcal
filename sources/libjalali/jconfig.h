/*
 * jconfig.h - Tools for manipulating Jalali representation of Iranian calendar
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

#ifndef JCONFIG_H
#define JCONFIG_H

#define JALALI_LEAP_BASE 475 /* Jalali 2820 year period epoch. */
#define JALALI_LEAP_PERIOD 2820 /* Jalali recurring pattern length. */

#define JALALI_NORMAL_CYCLE_LENGTH 128 /* A normal cycle length. */
#define JALALI_EXTRA_CYCLE_LENGTH 132 /* Last cycle length. */

/* Starting year of the last cycle in the period. */
#define JALALI_LAST_CYCLE_START 2688

#define J_PT0 0 /* No partitions passed. */
#define J_PT1 29 /* First partition passed. 0+29 */
#define J_PT2 62 /* Second partition passed. 0+29+33 */
#define J_PT3 95 /* Third partition passed. 0+29+33+33 */

#define J_L0 0
#define J_L1 7
#define J_L2 15
#define J_L3 23

#define J_C1 29 /* First type of partitions, 29 years in length. */
#define J_C2 33 /* Second type of partitions, 33 years in length. */
#define J_C2e 37 /* Extension to the second type, only one instance. */

#define J_LI 4 /* Multiples of four are leap except for zero. */

#define JALALI_TOTAL_LEAPS_IN_PERIOD 683
#define JALALI_LEAPS_EXCLUDING_LAST_CYCLE 651
#define JALALI_LEAPS_IN_NORMAL_CYCLE 31
#define JALALI_LEAPS_IN_EXTENDED_CYCLE 32
#define JALALI_NORMAL_CYCLE_LENGTH 128
#define JALALI_EXTENDED_CYCLE_LENGTH 132

#define J_DAY_LENGTH_IN_SECONDS 86400
#define J_DAY_LENGTH_IN_HOURS 24
#define J_HOUR_LENGTH_IN_SECONDS 3600
#define J_HOUR_LENGTH_IN_MINUTES 60
#define J_MINUTE_LENGTH_IN_SECONDS 60

#define J_WEEK_LENGTH 7
#define J_YEAR_LENGTH_IN_MONTHS 12

#define JALALI_LEAP_YEAR_LENGTH_IN_DAYS 366
#define JALALI_NORMAL_YEAR_LENGTH_IN_DAYS 365

#define J_UTC_EPOCH_YEAR 1348
#define J_UTC_EPOCH_MONTH 10
#define J_UTC_EPOCH_DAY 11
#define J_UTC_EPOCH_DIFF 286
#define J_UTC_EPOCH_WDAY 5

#endif /* JCONFIG_H */

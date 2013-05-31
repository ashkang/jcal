/*
 * jdate.h - Unix date-like interface to libjalali.
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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with jcal.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef JDATE_H
#define JDATE_H

#define JDATE_VERSION "0.4.1"

/* short options */
#define JDATE_VALID_ARGS "a:r:d:j:g:RuhV"

/* long options */
#define DATE_OPT "date"
#define REF_OPT "reference"
#define ACC_OPT "access"
#define JALALI_OPT "jalali"
#define GREGORIAN_OPT "gregorian"
#define RFC2822_OPT "rfc-2822"
#define UTC_OPT "utc"
#define UNIVERSAL_OPT "universal"
#define HELP_OPT "help"
#define VERSION_OPT "version"

/* help string */
#define HELP_STR "Usage: jdate [arRuhV]... [+OUTPUT_FORMAT]\
[d INPUT_FORMAT;DATE_STRING]\n\
Display the current date and time in the given FORMAT.\n\
\n\
  -d, --date=FORMAT;STRING\tdisplay time described by STRING, not `now'\n\
  -a, --access=FILE\t\tdisplay the last access time of FILE.\n\
  -r, --reference=FILE\t\tdisplay the last modification time of FILE.\n\
\n\
  -j, --jalali=%Y/%m/%d\t\tconverts a gregorian date to jalali.\n\
  -g, --gregorian=%Y/%m/%d\tconverts a jalali date to gregorian.\n\
\n\
  -R, --rfc-2822\t\toutput date and time in RFC 2822 format.\n\
\t\t\t\tExample: Jom, 06 Khor 1390 13:44:56 -0430.\n\
  -u, --utc, --universal\tprint Coordinated Universal Time.\n\
  -h, --help\t\t\tdisplay this help and exit.\n\
  -V, --version\t\t\toutput version information and exit.\n\
\n\
  FORMAT controls the output.  Interpreted sequences are:\n\
\n\
  %%   a literal %\n\
  %a   abbreviated weekday name (e.g., Sun)\n\
  %A   full weekday name (e.g., Sunday)\n\
  %b   abbreviated month name (e.g., Khor)\n\
  %B   full month name (e.g., Khordad)\n\
  %c   date and time (e.g., Jome Kho  6 17:18:25 1390)\n\
  %C   century; like %Y, except omit last two digits (e.g., 13)\n\
  %d   day of month (e.g., 01)\n\
  %D   date; same as %Y/%m/%d\n\
  %e   day of month, space padded; same as %d\n\
  %E   date and time in Farsi (utf8)\n\
  %F   full date; same as %Y-%m-%d\n\
  %h   abbreviated weekday name in Farsi transliteration (e.g. Jom)\n\
  %g   abbreviated Farsi weekday name (utf8)\n\
  %G   full Farsi weekday name (utf8)\n\
  %v   abbreviated Farsi month name (utf8)\n\
  %V   full Farsi month name (utf8)\n\
  %H   hour (00..23)\n\
  %I   hour (01..12)\n\
  %j   day of year (001..366)\n\
  %k   hour (0..23)\n\
  %l   hour (1..12)\n\
  %m   month (01..12)\n\
  %M   minute (00..59)\n\
  %n   a newline\n\
  %O   AM or PM notation for time in Farsi (utf8)\n\
  %p   either AM or PM; blank if not known\n\
  %P   like %p, but lower case\n\
  %r   12-hour clock time (e.g., 17:24:04 PM)\n\
  %R   24-hour hour and minute; same as %H:%M\n\
  %s   seconds since 1970-01-01 00:00:00 UTC\n\
  %S   second (00..59)\n\
  %t   a tab\n\
  %T   time; same as %H:%M:%S\n\
  %u   day of week (1..7); 1 is Saturday\n\
  %U   week number of year, with Saturday as first day of week (00..53)\n\
  %w   day of week (0..6); 0 is Saturday\n\
  %W   date representation in Farsi (utf8)\n\
  %x   date representation (e.g., 06/03/90)\n\
  %X   time representation in Farsi (utf8)\n\
  %y   last two digits of year (00..99)\n\
  %Y   year\n\
  %z   +hhmm numeric time zone (e.g., +0330)\n\
  %Z   alphabetic time zone abbreviation (e.g., IRST)\n\
\n\
Report jdate bugs to ghassemi@ftml.net\n\
libjalali home page: <http://savannah.nongnu.org/projects/jcal/>"

/*
 *@action_handler
 */
struct jdate_action {
    int normal;           /* standard representation: %h %b %m %H:%M:%S %Z %Y */
    int reference;        /* last modification time */
    char* reference_ptr;  /* last modification time argument */
    int access;           /* last access */
    char* access_ptr;     /* last access argument */
    int date;             /* use date string instead of 'now' */
    char* date_ptr;       /* date string argument */
    int jalali;           /* convert a gregorian date to jalali */
    char* jalali_ptr;     /* jalali conversion argument */
    int gregorian;        /* convert a jalali date to gregorian */
    char* gregorian_ptr;  /* gregorian conversion argument */
    int format;           /* +FORMAT. uses jstrftime() to format output */
    char* format_ptr;     /* +FORMAT argument */
    int rfc2822;          /* rfc2822 date and time: %h, %m %b %Y %H:%M:%S %z */
    int utc;              /* Coordinated Universal Time */
    int help;             /* help */
    int version;          /* version */
};

#endif /* JDATE_H */

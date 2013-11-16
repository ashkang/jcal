/*
 * termcap.h - Unix cal-like interface to libjalali.
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

#ifndef TERMCAP_H
#define TERMCAP_H

#define CURSOR_UP "\033[%dA"
#define CURSOR_DOWN "\033[%dB"
#define CURSOR_RIGHT "\033[%dC"
#define CURSOR_LEFT "\033[%dD"
#define CURSOR_C "\033b\033[%dC"

#if !defined NO_COLOR
/* Foreground colors on black. */
#define TERM_BLACK "\033[30m"
#define TERM_RED "\033[31m"
#define TERM_GREEN "\033[32m"
#define TERM_YELLOW "\033[33m"
#define TERM_BLUE "\033[34m"
#define TERM_MAGENTA "\033[35m"
#define TERM_CYAN "\033[36m"
#define TERM_WHITE "\033[37m"

/* Mixed sets. */
#define TERM_WHITE_ON_RED "\033[41;37m"
#define TERM_BLACK_ON_WHITE "\033[30;47m"
#define TERM_RED_ON_WHITE "\033[31;47m"

/* Attribute reset.*/
#define TERM_RESET "\033[0m"

#else /* NO_COLORS */

#define TERM_BLACK ""
#define TERM_RED ""
#define TERM_GREEN ""
#define TERM_YELLOW ""
#define TERM_BLUE ""
#define TERM_MAGENTA ""
#define TERM_CYAN ""
#define TERM_WHITE ""

#define TERM_WHITE_ON_RED ""
#define TERM_BLACK_ON_WHITE ""
#define TERM_RED_ON_WHITE ""

#define TERM_RESET ""

#endif /* !NO_COLOR */

#endif /* TERMCAP_H */

bin_PROGRAMS = elc get_date get_diff jalali_update jyinfo leap sec_converter

INCLUDES = -I../../libjalali

AM_CFLAGS = @CFLAGS@ -fno-inline -D_REENTRANT -Wall \
	-O2 -D_FILE_OFFSET_BITS=64 -D_LARGEFILE_SOURCE

elc_SOURCES = elc.c
get_date_SOURCES = get_date.c
get_diff_SOURCES = get_diff.c
jalali_update_SOURCES = jalali_update.c
jyinfo_SOURCES = jyinfo.c
leap_SOURCES = leap.c
sec_converter_SOURCES = sec_converter.c

LDADD           = ../../libjalali/libjalali.la

bin_PROGRAMS = jasctime jctime jgmtime jstrftime jstrptime jlocaltime jmktime

INCLUDES = -I../../libjalali

AM_CFLAGS = @CFLAGS@ -fno-inline -D_REENTRANT -Wall \
	-O2 -D_FILE_OFFSET_BITS=64 -D_LARGEFILE_SOURCE

jasctime_SOURCES = jasctime.c
jctime_SOURCES = jctime.c
jgmtime_SOURCES = jgmtime.c
jstrftime_SOURCES = jstrftime.c
jstrptime_SOURCES = jstrptime.c
jlocaltime_SOURCES = jlocaltime.c
jmktime_SOURCES = jmktime.c

LDADD           = ../../libjalali/libjalali.la -lreadline

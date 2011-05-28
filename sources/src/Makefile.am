bin_PROGRAMS = jcal jdate

INCLUDES = -I. -I.. -I../libjalali -I@includedir@ 

AM_CFLAGS = @CFLAGS@ -fno-inline -D_REENTRANT -Wall -O2 -D_FILE_OFFSET_BITS=64 -D_LARGEFILE_SOURCE

jcal_SOURCES = jcal.c 
jdate_SOURCES = jdate.c 
LDADD           = ../libjalali/libjalali.la -L@libdir@
LIBS            = @LIBS@ $(THREAD_LIBS) 


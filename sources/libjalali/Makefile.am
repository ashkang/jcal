#------------------------------------------------------------------------------
# Process this file with automake to produce Makefile.in.
#------------------------------------------------------------------------------

lib_LTLIBRARIES = libjalali.la

libjalali_la_SOURCES = jalali.c jtime.c

# 0:0:0
# 0 -> interface version, changes whenever you change the API
# 0 -> changes whenever you make a revision of an interface no
#      API changes...
# 0 -> changes whenever you change the API but keep it backwards
#      compatible (have not removed a function from the API, for
#      example...)

libjalali_la_LDFLAGS = -version-info 0:2:0
includedir= $(prefix)/include/jalali
include_HEADERS = jalali.h jtime.h jconfig.h
INCLUDES        =  -I. -I@includedir@
LIBS            = @LIBS@ $(THREAD_LIBS)
AM_CFLAGS          = @CFLAGS@ -D_REENTRANT -W -Wall -O2

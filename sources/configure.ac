#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_INIT([jcal], [0.4], [ghassemi@ftml.net])

AM_INIT_AUTOMAKE
AC_PROG_LIBTOOL
AC_CONFIG_MACRO_DIR([m4])

# Checks for programs.
AC_PROG_CC
AC_PROG_INSTALL
AC_PROG_MAKE_SET
AC_PATH_PROG(CP, cp, /bin/cp)
AC_PATH_PROG(RM, rm, /bin/rm)
AC_PATH_PROG(RMDIR, rmdir, /bin/rmdir)

# Checks for header files.
AC_CHECK_HEADERS([time.h limits.h stdlib.h string.h sys/time.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_TYPE_SIZE_T
AC_STRUCT_TIMEZONE

# Checks for library functions.
AC_FUNC_MALLOC
AC_FUNC_MKTIME
AC_CHECK_FUNCS([gettimeofday localtime_r memset strcasecmp strchr strstr tzset])

AC_CONFIG_FILES([Makefile
                 libjalali/Makefile
                 man/Makefile
                 src/Makefile
                ])

PYTHON_COMPAT=( python2.7 python2.6 )

AC_ARG_ENABLE([pyjalali], 
              AC_HELP_STRING([--enbale-pyjalali], [Install Python bindings (pyjalali)]),
              [case $enableval in
                      yes|no) installpyjalali=$enableval ;;
                      *) AC_MSG_ERROR([Bad value $enableval for --enable-pyjalali]) ;;
              esac], installpyjalali="no")

AC_ARG_WITH([python],
              AC_HELP_STRING([--with-python], [Python implementation to install python bindings for]),
              [foundimpl=0
               withpython=$withval
               for impl in ${PYTHON_COMPAT@<:@@@:>@}; do
                      test "x$impl" = "x$withval" && foundimpl=1
               done
               test $foundimpl -eq 1 || test "x$withpython" = "xauto" || AC_MSG_ERROR([
                      Unsupported python implementation $withval supplied by --with-python])
               unset foundimpl], withpython="auto")

if test $installpyjalali = "yes"; then
                if test "x$withpython" == "xauto"; then
                      AC_PATH_PROG(PYTHON, python ${PYTHON_COMPAT@<:@@@:>@} )
                else
                      PYTHON=$withpython
                fi
                AM_PATH_PYTHON
fi
AM_CONDITIONAL([WANT_PYJALALI], [test $installpyjalali = "yes"])

AC_OUTPUT

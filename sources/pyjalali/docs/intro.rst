Introduction
============

This page introduce pyjalali to you.  If you want rather head into class and
methods' documentation see :ref:`modindex`.

Install
-------

In jcal source directory navigate to subdirectory *sources* and run ``python
setup.py install``.  You must have libjalali installed before.

Usage
-----
You can use most functionalities of standard datetime module by
:mod:`pyjalali.datetime`:

  >>> from pyjalali.datetime import datetime
  >>> datetime.now()
  ... pyjalali.datetime.datetime(1392, 12, 1, 23, 40, 18, 772116)
  >>> datetime.today() - datetime(1367, 11 1)
  ... datetime.timedelta(30, 85313, 805141)

There is also :class:`pyjalali.datetime.date`:

  >>> from pyjalali.datetime import date
  >>> date.today().isoformat()
  ... '1392-12-01'

You can make timezone aware datetimes:
  >>> from pytz import timezone
  >>> now_in_teh = datetime.now(timezone('Asia/Tehran'))
  >>> # see current time in another area
  >>> in_amsterdam = now_in_teh.astimezone(timezone('Europe/Amsterdam'))
  >>> print now_in_teh.time(), in_amsterdam.time()
  ... 10:42:03.233978 08:12:03.233978
  >>> in_amsterdam == now_in_teh
  ... True

To convert between Hijri Shamsi and Gregorian calendars, you can use functions
:func:`~.datetime.jalali_from_gregorian` and
:func:`~.datetime.gregorian_from_jalali` (or respectively
:func:`~.datetime.g2j` and :func:`~.datetime.j2g` for short).  Also you can use
:attr:`.datetime.datetime.gregorian` to get equal datetime in Gregorian
calendar.  This will be cached for future uses:

  >>> from datetime import datetime as _std_datetime, date as _std_date
  >>> from pyjalali.datetime import jalali_from_gregorian, gregorian_from_jalali, datetime, date
  >>> jd = datetime(1389, 21, 11, 12, 23, 10)
  >>> gd = jd.gregorian
  >>> print gd, jd
  ... 2011-01-30 18:23:10  1389-11-10 18:23:10
  >>> gregorian_from_jalali(jd) == gd
  ... True
  >>> jalali_from_gregorian(_std_date.today())
  ... pyjalali.datetime.date(1392, 12, 2)

Note unlike many Hijri Shamsi implementations, libjalali doesn't work by
converting dates to Gregorian and forwarding operations to Gregorian date.
It's a Hijri Shamsi calendar implementation from base.  The only place
pyjalali converts Hijri Shamsi date to Gregorian internally, is to work with
`pytz`_ package in timezone aware datetimes.  So keep in mind libjalali
doesn't offer a general converter for dates before UNIX Epoch and pyjalali
converters will fail for them.

.. _pytz: http://pypi.python.org/pypi/pytz 

If you need to invoke libjalali library functions directly, you can use wrapper
functions.  :mod:`~.jtime` contains libjalali time functions.  Functions used
internally by libjalali resides in :mod:`~.jalali`.  :mod:`~.jstr` contains
functions for converting between dates and string representation.  Core
libjalali data structures are simulated in :mod:`~.types`.

  >>> from pyjalali.jtime import jlocaltime
  >>> print jlocaltime(int(time()))
  ... 1392/11/2 11-29-11 +12600 (IRST)

.. note::
  You can use functions :func:`.datetime.now` and :func:`.datetime.utcnow` in module level:
    >>> from pyjalali.datetime import utcnow
    >>> utcnow()
    ... pyjalali.datetime.datetime(1392, 12, 1, 20, 18, 27, 651499)

Version
-------
pyjalali is a very thin wrapper around libjalali.  Package's version you see
in :attr:`pyjalali.__init__.__version__` cosists of 3 numbers describing
version of libjalali which pyjalali supposed to work with it and another
number that is revision of pyjalali itself.  So ``0.6.11.92`` would expected
to work with libjalali version ``0.6.11``.

"""
    pyjalali.datetime
    ~~~~~~~~~~~~~~~~~

    High level API for libjalali.

    .. Note ::
        * There is no `iso_calendar` or `to_ordinal` or `fromordinal` method
          here.  Implementing them is easy, just forward to
          :attr:`.datetime.gregorian` but those methods are not related to
          Jalali really.
"""

from __future__ import absolute_import
import datetime as _std_dt_mod
from time import time as _timestamp, mktime, strftime

from pyjalali.jalali import (jalali_update, jalali_create_days_from_date,
                             jalali_year_month_days)
from pyjalali.jtime import jctime, jgmtime, jlocaltime, jmktime
from pyjalali.jstr import jstrftime, jstrptime
from pyjalali.types import struct_jtm, jtm_to_struct_time
from pyjalali.helpers import normalize_jtm


__all__ = ('date', 'datetime', 'j2g', 'g2j', 'now', 'utcnow',
           'jalali_from_gregorian', 'gregorian_from_jalali',
           'datetime_from_ts')


class date(object):
    __have_yday_wday = False
    __hash_val = None

    def __init__(self, year, month, day):
        if not 1 <= month <= 12:
            raise ValueError('month value out of range [1, 12]')
        if day > jalali_year_month_days(year, month - 1):
            raise ValueError('day is out of range for month')
        self.__jtm = struct_jtm()
        self.__jtm.tm_year = year
        self.__jtm.tm_mon = month - 1
        self.__jtm.tm_mday = day

    def __add__(self, delta):
        if isinstance(delta, _std_dt_mod.timedelta):
            njtm = self.__jtm.copy()
            njtm.tm_mday += delta.days
            normalize_jtm(njtm)
            return date(njtm.tm_year, njtm.tm_mon + 1, njtm.tm_mday)
        raise TypeError('Unsupported operand type for +: %s and %s' %
                        (self.__class__.__name__, delta.__class__.__name__))

    def __eq__(self, jdate):
        if isinstance(jdate, date):
            return (self.year == jdate.year and self.month == jdate.month and
                    self.day == jdate.day)
            raise TypeError('Unsupported operand type for ==: %s and %s' %
                            (self.__class__.__name__,
                             jdate.__class__.__name__))

    def __hash__(self):
        if self.__hash_val is None:
            self.__hash_val = hash((self.year, self.month, self.day, 101))
        return self.__hash_val

    def __lt__(self, jdate):
        if isinstance(jdate, date):
            if self.year < jdate.year:
                return True
            elif self.year != jdate.year:
                return False
            if self.month < jdate.month:
                return True
            elif self.month != jdate.month:
                return False
            return True if self.day < jdate.day else False
        raise TypeError('Unsupported operand type for <: %s and %s' %
                        (self.__class__.__name__, jdate.__class__.__name__))

    def __sub__(self, delta_or_date):
        if isinstance(delta_or_date, _std_dt_mod.timedelta):
            delta = delta_or_date
            njtm = self.__jtm.copy()
            njtm.tm_mday -= delta.days
            normalize_jtm(njtm)
            return date(njtm.tm_year, njtm.tm_mon + 1, njtm.tm_mday)
        if isinstance(delta_or_date, date):
            jd = delta_or_date
            onjtm = jd.jtm.copy()
            self._compute_yday_wday_if_necessary()
            jalali_create_days_from_date(onjtm)
            dx = self.__jtm.tm_yday - onjtm.tm_yday
            return _std_dt_mod.timedelta(days=dx)
        raise TypeError('Unsupported operand type for -: %s and %s' %
                        (self.__class__.__name__,
                         delta_or_date.__class__.__name__))

    def __repr__(self):
        return '%s.%s(%d, %d, %d)' % (self.__module__,
                                      self.__class__.__name__,
                                      self.year,
                                      self.month,
                                      self.day)

    def __str__(self):
        return self.isoformat()

    def __format__(self, format):
        return self.strftime(format)

    def _compute_yday_wday_if_necessary(self):
        if not self.__have_yday_wday:
            njtm = self.__jtm.copy()
            jalali_update(njtm)
            self.__jtm.tm_wday = njtm.tm_wday
            self.__jtm.tm_yday = njtm.tm_yday
            self.__have_yday_wday = True

    @property
    def jtm(self):
        """Broken-down jalali time structure for this date"""
        return self.__jtm

    @property
    def year(self):
        return self.__jtm.tm_year

    @property
    def month(self):
        return self.__jtm.tm_mon + 1

    def ctime(self):
        """Return a string representing the date, ``date(1392, 8, 2).ctime() ==
        'Thu Aba 02 00:00:00 1392'``"""
        njtm = self.__jtm.copy()
        jalali_update(njtm)
        return jctime(jmktime(njtm))

    @property
    def day(self):
        return self.__jtm.tm_mday

    def replace(self, **kw):
        """Return new date object where values for supplied keyword keywrod
        arguments reset: ``date(1392, 2, 8).replace(month=9) == date(1392, 9,
        8)```"""
        d = dict(year=self.year, month=self.month, day=self.day)
        d.update(**kw)
        return date(**d)

    @classmethod
    def today(self):
        """Return current local date"""
        return date.fromtimestamp(_timestamp())

    def timetuple(self):
        """Return a :class:`time.struct_time` from this date.  DST flag is
        -1"""
        # TODO: change after jmktime fix
        njtm = self.__jtm.copy()
        jalali_update(njtm)
        njtm.tm_isdst = -1
        return jtm_to_struct_time(njtm)

    @classmethod
    def fromtimestamp(self, ts):
        """Return the local date corresponding to the POSIX timestamp"""
        jtm = jlocaltime(int(ts))
        return date(jtm.tm_year, jtm.tm_mon + 1, jtm.tm_mday)

    def isoformat(self):
        """Return a string representing the date in ISO 8601 format,
        `YYYY-MM-DD`.  For example ``date(1392, 8, 2).isoformat() ==
        1392-08-02'``"""
        return self.strftime('%Y-%m-%d')

    def isoweekday(self):
        """Return the day of the week as an integer, where Shanbeh is 1"""
        return self.weekday() + 1

    def strftime(self, format):
        """Return a string representing the date, controlled by an explicit
        format string.  Format codes referring to hours, minutes or seconds
        will see 0 values.

        .. Warning::
            libjalali's :func:`jstrftime` defines custom formatting directives
            which might defined in your platform too but for other intentions.
            Check list of libjalali's formatting directives.
        """
        self._compute_yday_wday_if_necessary()
        return jstrftime(format, self.__jtm)

    def weekday(self):
        """Return the day of the week as an integer, where Shanbeh is 0"""
        self._compute_yday_wday_if_necessary()
        return self.__jtm.tm_wday


date.min = date(1, 1, 1)
date.max = date(9999, 12, 29)
date.resolution = _std_dt_mod.timedelta(days=1)


class datetime(object):
    __hash_val = None
    tzinfo = None  # let be here, might be invoked before init carelessly

    def __init__(self, year, month, day, hour=None, minute=None, second=None,
                 microsecond=0, tzinfo=None):
        if not isinstance(microsecond, int):
            raise TypeError
        self.__date = date(year, month, day)
        self.__jtm = self.__date.jtm
        if hour is not None:
            if not 0 <= hour <= 23:
                raise ValueError('hour must be in 0..23')
            self.__jtm.tm_hour = hour
        if minute is not None:
            if not 0 <= minute <= 59:
                raise ValueError('minute must be in 0..59')
            self.__jtm.tm_min = minute
        if second is not None:
            if not 0 <= second <= 59:
                raise ValueError('second must be in 0..59')
            self.__jtm.tm_sec = second
        self.microsecond = microsecond
        self.tzinfo = tzinfo

    def __add__(self, delta):
        if isinstance(delta, _std_dt_mod.timedelta):
            njtm = self.__jtm.copy()
            njtm.tm_sec += delta.seconds
            njtm.tm_mday += delta.days
            ms = normalize_jtm(njtm, self.microsecond + delta.microseconds)
            return datetime_from_jtm(njtm, ms, self.tzinfo)
        raise TypeError('Unsupported operand type for +: %s and %s' %
                        (self.__class__.__name__, delta.__class__.__name__))

    def __eq__(self, jdt):
        if isinstance(jdt, datetime):
            if (jdt.tzinfo is None) != (self.tzinfo is None):
                raise TypeError("can't compare offset-naive and offset-aware"
                                "datetime")
            if jdt.tzinfo != self.tzinfo:
                return (self.replace(tzinfo=None) - self.utcoffset()) == \
                       (jdt.replace(tzinfo=None) - jdt.utcoffset())
            return (self.year == jdt.year and self.month == jdt.month and
                    self.day == jdt.day and self.hour == jdt.hour and
                    self.minute == jdt.minute and self.second == jdt.second
                    and self.microsecond == jdt.microsecond)
        raise TypeError('Unsupported operand type for ==: %s and %s' %
                        (self.__class__.__name__, jdt.__class__.__name__))

    def __hash__(self):
        # tzinfo shoudn't count, two date with different zone's should produce
        # same hash but if one is aware and one is naive it should be different
        if self.__hash_val is None:
            if self.tzinfo is None:
                d = self
            else:
                d = self - self.utcoffset()
            # yes, who cares?
            str = '%d%d%d%d%d%d%d%d' % (d.year, d.month, d.day, d.hour,
                                        d.minute, d.second, d.microsecond,
                                        d.tzinfo is None)
            self.__hash_val = hash(str)
        return self.__hash_val

    def __lt__(self, dt):
        if isinstance(dt, datetime):
            d1 = self
            d2 = dt
            if (d1.tzinfo or d2.tzinfo) is None:
                if d1.year < d2.year:
                    return True
                if d1.year != d2.year:
                    return False
                if d1.month < d2.month:
                    return True
                if d1.month != d2.month:
                    return False
                if d1.day < d2.day:
                    return True
                elif d1.day != d2.day:
                    return False
                if d1.hour < d2.hour:
                    return True
                elif d1.hour != d2.hour:
                    return False
                if d1.minute < d2.minute:
                    return True
                elif d1.minute != d2.minute:
                    return False
                if d1.second < d2.second:
                    return True
                elif d1.second != d2.second:
                    return False
                return True if d1.microsecond < d2.microsecond else False
            if not (d1.tzinfo and d2.tzinfo):
                raise TypeError("can't compare offset-naive and offset-aware"
                                "datetimes")
            if d1.tzinfo is not None:
                d1 = d1.replace(tzinfo=None) - d1.utcoffset()
            if d2.tzinfo is not None:
                d2 = d2.replace(tzinfo=None) - d2.utcoffset()
            return d1 < d2
        if isinstance(dt, _std_dt_mod.datetime):
            # It might seem stupid but pytz needs this anyway
            return self.gregorian < dt
        raise TypeError('Unsupported operand type for <: %s and %s' %
                        (self.__class__.__name__, dt.__class__.__name__))

    def __repr__(self):
        fmt = '%s.%s(%r, %r, %r, %r, %r, %r, %r%%s' % \
              (self.__module__,
               self.__class__.__name__,
               self.year,
               self.month,
               self.day,
               self.hour,
               self.minute,
               self.second,
               self.microsecond)
        if self.tzinfo is not None:
            return fmt % (', tzinfo=%r)' % self.tzinfo)
        return fmt % ')'

    def __str__(self):
        return self.isoformat(' ')

    def __sub__(self, delta_or_jdt):
        if isinstance(delta_or_jdt, _std_dt_mod.timedelta):
            delta = delta_or_jdt
            njtm = self.__jtm.copy()
            njtm.tm_sec -= delta.seconds
            njtm.tm_mday -= delta.days
            ms = normalize_jtm(njtm, self.microsecond - delta.microseconds)
            return datetime_from_jtm(njtm, ms, self.tzinfo)
        if isinstance(delta_or_jdt, _std_dt_mod.datetime):
            raise TypeError("It doesn't make sense subtract Gregorian date "
                            "from Jalali date")
        if isinstance(delta_or_jdt, datetime):
            jdt = delta_or_jdt
            if (self.tzinfo is None) != (jdt.tzinfo is None):
                raise TypeError("can't subtract offset-naive and offset-aware"
                                "datetimes")
            if self.tzinfo != jdt.tzinfo:
                return (self.replace(tzinfo=None) - self.utcoffset()) - \
                       (jdt.replace(tzinfo=None) - jdt.utcoffset())
            self.__date._compute_yday_wday_if_necessary()
            onjtm = jdt.jtm.copy()
            jalali_create_days_from_date(onjtm)
            dx = _std_dt_mod.timedelta(days=self.__jtm.tm_yday-onjtm.tm_yday,
                                       hours=self.hour - jdt.hour,
                                       minutes=self.minute - jdt.minute,
                                       seconds=self.second - jdt.second,
                                       microseconds=(self.microsecond -
                                                     jdt.microsecond))
            return dx
        raise TypeError('Unsupported operand type for -: %s and %s' %
                        (self.__class__.__name__,
                         delta_or_jdt.__class__.__name__))

    def astimezone(self, tz):
        """
        Return a :class:`.datetime` object with new :attr:`.tzinfo` attribute
        *tz*, adjusting the date and time data so the result is the same UTC
        time as self, but in *tz*'s local time.

        >>> from pytz import timezone
        >>> d1 = datetime.now(timezone('Asia/Tehran'))
        >>> d2 = d1.astimezone(timezone('Asia/Dubai'))
        >>> d1 == d2, d1 - d2
        (True, datetime.timedelta(0))
        """
        if self.tzinfo is None:
            raise ValueError('astimezone() cannot be applied to naive-offset '
                             'datetime')
        utc = (self - self.utcoffset()).replace(tzinfo=None)
        return tz.fromutc(utc)

    def dst(self):
        """If :attr:`.tzinfo` is None, returns None, else returns
        ``self.tzinfo.dst(self)``, and raises an exception if the latter
        doesn't return None, or a timedelta object.
        """
        if self.tzinfo is None:
            return
        rv = self.tzinfo.dst(self)
        if rv is None or isinstance(rv, _std_dt_mod.timedelta):
            return rv
        raise TypeError("tzinfo.dst() must return None or a timedelta, not "
                        "'%s'" % rv.__class__.__name__)

    @property
    def gregorian(self):
        """Return Gregorian :class:`python:datetime.datetime` object
        corresponding to this datetime.  Result will cached for future uses.
        """
        if getattr(self, '__gregorian', None) is None:
            self.__gregorian = gregorian_from_jalali(self)
        return self.__gregorian

    @property
    def year(self):
        return self.__jtm.tm_year

    @property
    def month(self):
        return self.__jtm.tm_mon + 1

    @property
    def day(self):
        return self.__jtm.tm_mday

    @property
    def hour(self):
        return self.__jtm.tm_hour

    @property
    def minute(self):
        return self.__jtm.tm_min

    @property
    def second(self):
        return self.__jtm.tm_sec

    @classmethod
    def combine(self, date, time):
        """Make :class:`.datetime` from supplied date and time.

        :param `pyjalali.datetime.date` date:
        :param `datetime.time` time:
        """
        return datetime(date.year, date.month, date.day, time.hour,
                        time.minute, time.second, time.microsecond,
                        time.tzinfo)

    def ctime(self):
        """Return a string representing the date and time.

        >>> datetime(1392, 9, 1, 12, 32, 14, 992).ctime()
        'Fri Aza 01 12:32:14 1392'
        """
        # TODO:
        # this should be enough if jmktime fixed:
        # return jctime(jmktime(self.__jtm))
        njtm = self.__jtm.copy()
        jalali_update(njtm)
        return jctime(jmktime(njtm))

    def date(self):
        """Return :class:`.date` object with same year, month and day."""
        return self.__date.replace()

    @classmethod
    def fromtimestamp(self, ts, tz=None):
        """Return the local date and time corresponding to the POSIX
        timestamp, such as is returned by :func:`time.time`.  If optional
        argument *tz* is None or not specified, the timestamp is converted to
        the platform's local date and time, and the returned datetime object
        is naive.
        """
        return datetime_from_ts(ts, True, tz)

    @property
    def jtm(self):
        """Broken-down jalali time structure for this date"""
        return self.__jtm

    def isoformat(self, sep='T'):
        """
        Return a string representing the date and time in ISO 8601 format,
        YYYY-MM-DDTHH:MM:SS.mmmmmm or, if :attr`:`.microsecond` is 0,
        YYYY-MM-DDTHH:MM:SS.  The optional argument sep (default 'T') is a
        separator, placed between the date and time portions of the result.

        >>> datetime(1392, 9, 1, 12, 32, 14, 992).isoformat(' ')
        '1392-09-01 12:32:14.992'
        """
        format = '%Y-%m-%d'+sep+'%H:%M:%S'
        if self.microsecond != 0:
            format += '.%d' % self.microsecond
        utcoff = self.utcoffset()
        if utcoff is not None:
            if utcoff.total_seconds() >= 0:
                sign = '+'
            else:
                sign = '-'
            format += '%s%s' % (sign, str(utcoff).rsplit(':', 1)[0])
        return self.strftime(format)

    def isoweekday(self):
        """Return the day of the week as an integer, where Shanbeh is 1"""
        return self.__date.isoweekday()

    @classmethod
    def now(self, tz=None):
        """Return the current local date and time.  If a timezone provided,
        date and time will adjusted to that timezone."""
        return now(tz)

    def replace(self, **kw):
        """Return a datetime with the same attributes, except for those
        attributes given new values by whichever keyword arguments are
        specified.
        """
        # TODO: make it like std, with positional args
        d = dict(year=self.year, month=self.month, day=self.day,
                 hour=self.hour, minute=self.minute, second=self.second,
                 microsecond=self.microsecond, tzinfo=self.tzinfo)
        d.update(**kw)
        return datetime(**d)

    def strftime(self, format, _decrease_gmtoff_from_secs=False):
        """Return a string representing the date and time, controlled by an
        explicit format string.

        .. Note ::
            To show correct value for some formatting specials, e.g.'%s',
            libjalali's :func:`jstrftime` needs timezone informations filled
            in :attr:`.jtm.tm_gmtoff` (`issue 4`_) which we could not depend
            on here, since naive datetime objects have zero knowledge about
            it.  Storing these timezone information in a naive datetime
            object, make datetime implementation heavily depended on
            :func:`jmktime` which have several issues itself.  Additionally it
            makes :class:`~pyjalali.datetime.datetime` less like
            :class:`python:datetime.datetime` since it should store more
            information and change method signatures.

            For those directives, a possible workaround is to change
            :attr:`.jtm` by detecting local zone and putting its effect in
            other field values, but this makes other directives (hour, minute,
            etc) incorrect.  You can use this workaround by passing True as
            second argument.

            .. _issue 4: https://github.com/ashkang/jcal/issues/4
        """
        self.__date._compute_yday_wday_if_necessary()
        if self.tzinfo is not None:
            njtm = self.__jtm.copy()
            njtm.tm_gmtoff = int(self.utcoffset().total_seconds())
            njtm.tm_zone = self.tzname()
            if _decrease_gmtoff_from_secs:
                njtm.tm_sec += (njtm.tm_gmtoff -
                                jlocaltime(int(_timestamp())).tm_gmtoff)
            return jstrftime(format, njtm)
        if _decrease_gmtoff_from_secs:
            return jstrftime(format, self.__localtime_offset_decreased_jtm())
        return jstrftime(format, self.jtm)

    def __localtime_offset_decreased_jtm(self):
        # workaround for strftime GMT offset dependecy
        njtm = self.__jtm.copy()
        if self.tzinfo is None:
            njtm.tm_gmtoff = 0
            njtm.tm_sec -= jlocaltime(int(_timestamp())).tm_gmtoff
            njtm.tm_zone = ''
            normalize_jtm(njtm)
        return njtm

    @classmethod
    def strptime(self, date_str, format):
        """Return a datetime corresponding to `date_str`, parsed according to
        `format`."""
        return datetime_from_jtm(jstrptime(format, date_str))

    def timetuple(self):
        """Return a :class:`time.struct_time` from this date. The tm_isdst
        flag of the result is set according to the dst() method: tzinfo is
        None or dst() returns None, tm_isdst is set to -1; else if dst()
        returns a non-zero value, tm_isdst is set to 1; else tm_isdst is set
        to 0."""

        njtm = self.__jtm.copy()
        jalali_update(njtm)
        if self.dst() is None:
            njtm.tm_isdst = -1
        elif self.dst() == 0:
            njtm.tm_isdst = 0
        else:
            njtm.tm_isdst = 1
        return jtm_to_struct_time(njtm)

    def utctimetuple(self):
        if self.tzinfo is None:
            d = self
        else:
            d = self.replace(tzinfo=None) - self.utcoffset()
        njtm = d.jtm.copy()
        jalali_update(njtm)
        return jtm_to_struct_time(njtm)

    def time(self):
        """Return :class:`python:datetime.time` object with same hour, minute,
        second and microsecond.  :attr:`.tzinfo` is None."""
        return _std_dt_mod.time(self.hour, self.minute, self.second,
                                self.microsecond)

    def timetz(self):
        """Return :class:`python:datetime.time` object with same hour, minute,
        second, microsecond, and tzinfo attributes."""
        return _std_dt_mod.time(self.hour, self.minute, self.second,
                                self.microsecond, self.tzinfo)

    @classmethod
    def today(self):
        """Return the current local datetime, with :attr:`~.tzinfo` None."""
        return now()

    def tzname(self):
        """If tzinfo is None, returns None, else returns
        ``self.tzinfo.tzname(self)``, raises an exception if the latter
        doesn't return None or a string object.
        """
        if self.tzinfo is None:
            return
        rv = self.tzinfo.tzname(self)
        if rv is None or isinstance(rv, str):
            return rv
        raise TypeError("tzinfo.tzname() must return None or a str, not "
                        "'%s'" % self.__class__.__name__)

    def utcoffset(self):
        """
        Timezone offset.  If :attr:`~.tzinfo` is None, returns None, else
        returns ``self.tzinfo.utcoffset(self)``, and raises an exception if
        the latter doesn't return None, or a :py:class:`datetime.timedelta`
        object.

        >>> from pytz import timezone,AmbiguousTimeError,NonExistentTimeError
        >>> timezone('Asia/Tehran').utcoffset(datetime(1390, 1, 1, 10, 2))
        datetime.timedelta(0, 12600)
        >>> timezone('Asia/Tehran').utcoffset(datetime(1390, 6, 30, 22, 30))
        datetime.timedelta(0, 16200)
        >>> ambiguous = datetime(1392, 6, 30, 23, 30)
        >>> try:
        ...     timezone('Asia/Tehran').utcoffset(ambiguous)
        ... except AmbiguousTimeError:
        ...     print "caught"
        caught
        >>> non_existent = datetime(1390, 1, 2, 0, 45)
        >>> try:
        ...     timezone('Asia/Tehran').utcoffset(non_existent)
        ... except NonExistentTimeError:
        ...     print "caught"
        caught
        """
        if self.tzinfo is None:
            return
        rv = self.tzinfo.utcoffset(self)
        if rv is None or isinstance(rv, _std_dt_mod.timedelta):
            return rv
        raise TypeError("tzinfo.utcoffset() must return None or a timedelta, "
                        "not '%s'" % rv.__class__.__name__)

    @classmethod
    def utcfromtimestamp(self, ts):
        """Return the UTC :class:`~.datetime` corresponding to the POSIX
        timestamp, with :attr:`~.tzinfo` None."""
        return datetime_from_ts(ts, False)

    @classmethod
    def utcnow(self):
        """Return the current UTC date and time, with :attr:`~.tzinfo` None.
        This is like :meth:`~.now`, but returns the current UTC date and time,
        as a naive :class:`~.datetime` object."""
        return utcnow()

    def weekday(self):
        """Return the day of the week as an integer, where Shanbeh is 0."""
        return self.__date.weekday()


datetime.min = datetime(1, 1, 1)
#XXX: not sure, ask ashkan
datetime.max = datetime(9999, 12, 29, 23, 59, 59)
datetime.resolution = _std_dt_mod.timedelta(seconds=1)


def now(timezone=None):
    """See :meth:`.datetime.now`."""
    return datetime_from_ts(_timestamp(), True, tz=timezone)


def utcnow():
    """See :meth:`.datetime.utcnow`."""
    return datetime_from_ts(_timestamp(), False)


def datetime_from_ts(ts, local, tz=None):
    """Return :class:`.datetime` from provided timestamp `ts`.

    :param int ts: timestamp
    :param bool local: if True, return local date else return date in UTC
    :param `datetime.tzinfo` tz: if provided, make timezone aware datetime
        discarding effect of local parameter
    """
    uts = int(ts % 1 * 1000000)
    tts = int(ts)
    if local and tz is None:
        jtm = jlocaltime(tts)
    else:
        if local:
            return tz.fromutc(datetime.utcfromtimestamp(ts))
        jtm = jgmtime(tts)
    return datetime_from_jtm(jtm, uts, tz)


def jalali_from_gregorian(date_or_datetime):
    """Make Jalali :class:`.datetime` from Gregorian
    :class:`python:datetime.datetime` or make Jalali :class:`.date` from
    Gregorian :class:`python:datetime.date`.

    >>> from datetime import datetime as _dtm, date as _dt
    >>> jalali_from_gregorian(_dtm(2013, 11, 23, 23, 46, 0, 703498))
    pyjalali.datetime.datetime(1392, 9, 2, 23, 46, 0, 703498)
    >>> jalali_from_gregorian(_dtm(2013, 4, 13, 21, 10, 2, 292))
    pyjalali.datetime.datetime(1392, 1, 24, 21, 10, 2, 292)
    >>> jalali_from_gregorian(_dtm(2013, 3, 22, 0, 12))
    pyjalali.datetime.datetime(1392, 1, 2, 0, 12, 0, 0)
    >>> jalali_from_gregorian(_dt(2013, 3, 21))
    pyjalali.datetime.date(1392, 1, 1)
    """
    if isinstance(date_or_datetime, _std_dt_mod.datetime):
        gdate = date_or_datetime.date()
        time = date_or_datetime.time()
    elif isinstance(date_or_datetime, _std_dt_mod.date):
        gdate = date_or_datetime
        time = None
    else:
        raise TypeError('Expected Gregorian %s or %s instance, not %s' %
                        (_std_dt_mod.datetime.__name__,
                         _std_dt_mod.date.__name__,
                         date_or_datetime.__class__.__name__))
    if gdate < _std_dt_mod.date(1970, 1, 1):
        raise ValueError("Can't convert dates before Epoch")
    jdate = date.fromtimestamp(mktime(gdate.timetuple()))
    if time is None:
        return jdate
    return datetime.combine(jdate, time)


def gregorian_from_jalali(date_or_datetime):
    """
    Make Gregorian :class:`python:datetime.datetime` from Jalali
    :class:`.datetime` or make Gregorian :class:`python:datetime.date` from
    Jalali :class:`.date`.

    >>> gregorian_from_jalali(datetime(1392, 9, 2, 23, 10, 2))
    datetime.datetime(2013, 11, 23, 23, 10, 2)
    >>> gregorian_from_jalali(datetime(1392, 6, 30, 22, 30))
    datetime.datetime(2013, 9, 21, 22, 30)
    >>> gregorian_from_jalali(date(1392, 6, 30))
    datetime.date(2013, 9, 21)
    """
    if isinstance(date_or_datetime, datetime):
        jdate = date_or_datetime.date()
        time = date_or_datetime.timetz()
    elif isinstance(date_or_datetime, date):
        jdate = date_or_datetime
        time = None
    else:
        raise TypeError('Expected Jalali %s or %s instance, not %s' %
                        (datetime.__name__, date.__name__,
                         date_or_datetime.__class__.__name__))
    if jdate < date(1348, 10, 11):
        raise ValueError("Can't convert dates before Epoch")
    # to stop dst changes from bugging conversion, just convert date
    njtm = jdate.jtm.copy()
    jalali_update(njtm)  # pragma: jmktime needs yday TODO: remove
    gdate = _std_dt_mod.date.fromtimestamp(jmktime(njtm))
    if time is None:
        return gdate
    return _std_dt_mod.datetime.combine(gdate, time)


def datetime_from_jtm(jtm, microsecond=0, tz=None):
    return datetime(*jtm_to_struct_time(jtm)[:6], microsecond=microsecond,
                    tzinfo=tz)


g2j = jalali_from_gregorian
"""Alias for :func:`.jalali_from_gregorian`."""

j2g = gregorian_from_jalali
"""Alias for :func:`.gregorian_from_jalali`."""

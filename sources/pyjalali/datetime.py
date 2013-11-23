"""
    pyjalali.datetime
    ~~~~~~~~~~~~~~~~~

    High level API for libjalali.  Intended to be similar to standard
    data:`datetime` module.

    .. Note ::  
        * There is no `iso_calendar` or `to_ordinal` or `fromordinal` method
          here.  Implementing them is easy, just forward to
          :attr:`~pyjalali.datetime.gregorian` but those methods are not
          related to Jalali really

"""

from __future__ import absolute_import
import datetime as _std_dt_mod
from time import time as _timestamp, mktime, strftime

from pyjalali.jalali import jalali_update, jalali_create_days_from_date
from pyjalali.jtime import jctime_r, jgmtime_r, jlocaltime_r, jmktime
from pyjalali.jstr import jstrftime, jstrptime
from pyjalali.types import struct_jtm, jtm_to_struct_time
from pyjalali.helpers import normalize_jtm, get_yday

class date(object):
    __have_yday_wday = False
    def __init__(self, year, month, day):
        if not 1 <= month <= 12:
            raise ValueError('month value out of range [1, 12]')
        self.jtm = struct_jtm()
        self.jtm.tm_year = year
        self.jtm.tm_mon = month - 1
        self.jtm.tm_mday = day

    @property
    def year(self):
        return self.jtm.tm_year

    @property
    def month(self):
        return self.jtm.tm_mon + 1

    @property
    def day(self):
        return self.jtm.tm_mday

    @classmethod
    def today(self):
        return date.fromtimestamp(int(_timestamp()))

    @classmethod
    def fromtimestamp(self, ts):
        jmt = jlocaltime_r(ts)
        return date(jmt.tm_year, jtm.tm_mon + 1, jtm.tm_mday)

    def isoformat(self):
        return self.strftime('%Y-%m-%d')


date.min = date(1, 1, 1)
date.max = date(9999, 12, 29)

class datetime():
    """Differences with :class:`datetime.datetime`:
        * Microsecond could not specified here.

        * :attr:`strftime` and :attr:`strptime` accept customized libjalali
          formatting that differs with standard formatting.
    """

    jtm = None
    tzinfo = None
    __have_yday_wday = False
    __hash_val = None

    def __init__(self, year, month, day, hour=None, minute=None, second=None,
                 microsecond=0, tzinfo=None):
        if not isinstance(microsecond, int):
            raise TypeError
        self.__date = date(year, month, day)
        self.jtm = self.__date.jtm
        if hour is not None:
            self.jtm.tm_hour = hour
        if minute is not None:
            self.jtm.tm_min = minute
        if second is not None:
            self.jtm.tm_sec = second
        self.microsecond = microsecond
        self.tzinfo = tzinfo

    def __add__(self, delta):
        if isinstance(delta, _std_dt_mod.timedelta):
            njtm = self.jtm.copy()
            njtm.tm_sec += delta.seconds
            njtm.tm_mday += delta.days
            ms = normalize_jtm(njtm, self.microsecond + delta.microseconds) 
            return jdatetime_from_jtm(njtm, ms, self.tzinfo)
        raise TypeError('Unsupported operand type for +: %s and %s' % \
                        (self.__class__.__name__, delta.__class__.__name__))

    def __eq__(self, jdt):
        if isinstance(jdt, datetime):
            if (jdt.tzinfo is None) != (self.tzinfo is None):
                raise TypeError("can't compare offset-naive and offset-aware"\
                                "datetime")
            if jdt.tzinfo != self.tzinfo:
                return (self.replace(tzinfo=None) - self.utcoffset()) == \
                       (jdt.replace(tzinfo=None) - jdt.utcoffset())
            return self.year == jdt.year and self.month == jdt.month and \
                   self.day == jdt.day and self.hour == jdt.hour and \
                   self.minute == jdt.minute and self.second == jdt.second \
                   and self.microsecond == jdt.microsecond
        raise TypeError('Unsupported operand type for ==: %s and %s' % \
                        (self.__class__.__name__, jdt.__class__.__name__))

    def __hash__(self):
        # XXX:
        if self.__hash_val is None:
            self.__hash_val = self.year
            self.__hash_val = self.__hash_val * 100 + self.month
            self.__hash_val = self.__hash_val * 100 + self.day
            self.__hash_val = self.__hash_val * 100 + self.hour
            self.__hash_val = self.__hash_val * 100 + self.minute
            self.__hash_val = self.__hash_val * 100 + self.day
            self.__hash_val = self.__hash_val * 1000000 + self.second
            if self.tzinfo is not None:
                self.__hash_val += hash(self.tzinfo)
        return self.__hash_val

    def __lt__(self, dt):
        if isinstance(dt, datetime):
            return (self - dt).total_seconds() < 0
        if isinstance(dt, _std_dt_mod.datetime):
            # It might seem stupid but pytz needs this anyway
            return self.gregorian < dt
        raise TypeError('Unsupported operand type for <: %s and %s' % \
                        (self.__class__.__name__, dt.__class__.__name__))

    def __repr__(self):
        fmt = '%s.%s(%s, %s, %s, %s, %s, %s, %s%%s' % \
              (self.__module__,
               self.__class__.__name__,
               repr(self.year),
               repr(self.month),
               repr(self.day),
               repr(self.hour),
               repr(self.minute),
               repr(self.second),
               repr(self.microsecond))
        if self.tzinfo is not None:
            return fmt % ', tzinfo=%s)' % repr(self.tzinfo)
        return fmt % ')'

    def __str__(self):
        return self.isoformat(' ')

    def __sub__(self, delta_or_jdt):
        if isinstance(delta_or_jdt, _std_dt_mod.timedelta):
            delta = delta_or_jdt
            njtm = self.jtm.copy()
            njtm.tm_sec -= delta.seconds
            njtm.tm_mday -= delta.days
            ms = normalize_jtm(njtm, self.microsecond - delta.microseconds)
            return jdatetime_from_jtm(njtm, ms, self.tzinfo)
        if isinstance(delta_or_jdt, _std_dt_mod.datetime):
            raise TypeError("It doesn't make sense subtract Gregorian date " \
                            "from Jalali date")
        if isinstance(delta_or_jdt, datetime):
            jdt = delta_or_jdt
            if (self.tzinfo is None) != (jdt.tzinfo is None):
                raise TypeError("can't subtract offset-naive and offset-aware"\
                                "datetimes")
            if self.tzinfo != jdt.tzinfo:
                return (self.replace(tzinfo=None) - self.utcoffset()) - \
                       (jdt.replace(tzinfo=None) - jdt.utcoffset())
            if not self.__have_yday_wday:
                self._compute_wday_yday()
            onjtm = jdt.jtm.copy()
            jalali_create_days_from_date(onjtm)
            dx = _std_dt_mod.timedelta(days = self.jtm.tm_yday-onjtm.tm_yday,
                                       hours = self.hour - jdt.hour,
                                       minutes = self.minute - jdt.minute,
                                       seconds = self.second - jdt.second,
                                       microseconds = self.microsecond -
                                                      jdt.microsecond)
            return dx
        raise TypeError('Unsupported operand type for -: %s and %s' % \
                        (self.__class__.__name__, delta_or_jdt.__class__.__name__))

    def _compute_wday_yday(self):
        # DANGER: i'm not *very* sure about jalali_update here
        njtm = self.jtm.copy()
        jalali_update(njtm)
        self.jtm.tm_wday = njtm.tm_wday
        self.jtm.tm_yday = njtm.tm_yday
        self.__have_yday_wday = True
    
    def astimezone(self, tz):
        """
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
        if self.tzinfo is None:
            return
        rv = self.tzinfo.dst(self)
        if rv is None or isinstance(rv, _std_dt_mod.timedelta):
            return rv
        raise TypeError("tzinfo.dst() must return None or a timedelta, not " \
                        "'%s'" % rv.__class__.__name__)

    @property
    def gregorian(self):
        if getattr(self, '__gregorian', None) is None:
            self.__gregorian = dt_from_jdatetime(self)
        return self.__gregorian

    @property
    def year(self):
        return self.jtm.tm_year

    @property
    def month(self):
        return self.jtm.tm_mon + 1

    @property
    def day(self):
        return self.jtm.tm_mday

    @property
    def hour(self):
        return self.jtm.tm_hour

    @property
    def minute(self):
        return self.jtm.tm_min

    @property
    def second(self):
        return self.jtm.tm_sec

    @classmethod
    def combine(self, date, time):
        return datetime(self.year, self.month, self.day, self.hour,
               self.minute, self.second, self.microsecond, self.tzinfo)

    def ctime(self):
        # TODO:
        # this should be enough if jmktime fixed:
        # return jctime_r(jmktime(self.jtm))
        njtm = self.jtm.copy()
        jalali_update(njtm)
        return jctime_r(jmktime(njtm))

    def date(self):
        return self.__date

    @classmethod
    def fromtimestamp(self, ts, tz=None):
        return jdatetime_from_ts(ts, True, tz)

    def isoformat(self, sep='T'):
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

    @classmethod
    def now(self, tz=None):
        return now(tz)

    def replace(self, **kw):
        d = dict(year=self.year, month=self.month, day=self.day,
                 hour=self.hour, minute=self.minute, second=self.second,
                 microsecond=self.microsecond, tzinfo=self.tzinfo)
        d.update(**kw)
        return datetime(**d)

    def strftime(self, format):
        """.. Note :: To show correct value for some formatting specials, e.g.
        '%s' libjalali's :func:`jstrftime` needs timezone informations filled
        in struct_jtm.tm_gmtoff which we could not depend on here, since naive
        datetime objects have zero knowledge about it.  Storing these timezone
        information in a naive datetime object, make datetime implementation
        heavily depended on :func:`jmktime` which have several issues itself.
        Additionally it makes :class:`~pyjalali.datetime.datetime` less like
        :class:`datetime.datetime` since it should store more information and
        change method signatures.

        So consider returned values for every switch depended to timezone
        information, like '%s' is wrong.
        """
        if not self.__have_yday_wday:
            self._compute_wday_yday()        
        return jstrftime(format, self.jtm)
    
    @classmethod
    def strptime(self, date_str, format):
        return jdatetime_from_jtm(jstrptime(format, date_str))

    def timetuple(self):
        njtm = self.jtm.copy()
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
        return _std_dt_mod.time(self.hour, self.minute, self.second)
    
    def timetz(self):
        return _std_dt_mod.time(self.hour, self.minute, self.second,
                                self.tzinfo)

    @classmethod
    def today(self):
        return now()

    def tzname(self):
        if self.tzinfo is None:
            return
        rv = self.tzinfo.tzname(self)
        if rv is None or isinstance(rv, str):
            return rv
        raise TypeError("tzinfo.tzname() must return None or a str, not " \
                        "'%s'" % self.__class__.__name__)

    def utcoffset(self):
        """
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
        raise TypeError("tzinfo.utcoffset() must return None or a timedelta, "\
                        "not '%s'" % rv.__class__.__name__)

    @classmethod
    def utcfromtimestamp(self, ts):
        return jdatetime_from_ts(ts, False)

    @classmethod
    def utcnow(self):
        return utcnow()


datetime.min = datetime(1,1,1)
#XXX: not sure, ask ashkan
datetime.max = datetime(9999, 12, 29, 23, 59, 59)
datetime.resolution = _std_dt_mod.timedelta(seconds=1)

date.__format__ = datetime.__format__ = lambda s, f: s.strftime(f)
date.weekday = datetime.weekday = lambda self: self.jtm.tm_wday
date.isoweekday = datetime.isoweekday = lambda self: self.jtm.tm_wday + 1


def now(timezone=None):
    return jdatetime_from_ts(_timestamp(), True, tz=timezone)


def utcnow():
    return jdatetime_from_ts(_timestamp(), False)


def jdatetime_from_ts(ts, local, tz=None):
    uts = int(ts % 1 * 1000000)
    tts = int(ts)
    if local and tz is None:
        jtm = jlocaltime_r(tts)
    else:
        if local:
            return tz.fromutc(datetime.utcfromtimestamp(ts))
        assert tz is None
        jtm = jgmtime_r(tts)
    return jdatetime_from_jtm(jtm, uts, tz)


def jdatetime_from_dt(dt):
    """Make Jalali datetime from Gregorian datetime
    >>> from datetime import datetime as _dtm
    >>> jdatetime_from_dt(_dtm(2013, 11, 23, 23, 46, 0, 703498))
    pyjalali.datetime.datetime(1392, 9, 2, 23, 46, 0, 703498)
    >>> jdatetime_from_dt(_dtm(2013, 4, 13, 21, 10, 2, 292))
    pyjalali.datetime.datetime(1392, 1, 24, 21, 10, 2, 292)
    >>> jdatetime_from_dt(_dtm(2013, 3, 22, 0, 12))
    pyjalali.datetime.datetime(1392, 1, 2, 0, 12, 0, 0)
    """
    if not isinstance(dt, _std_dt_mod.datetime):
        raise TypeError('Expected %s instance' % _std_dt_mod.datetime)
    ndt = dt.replace(hour=12, minute=0, second=0) # no dst changes here
    ts = mktime(ndt.timetuple())
    return datetime.fromtimestamp(ts).replace(hour=dt.hour, minute=dt.minute,
           second=dt.second, microsecond=dt.microsecond)


def dt_from_jdatetime(jdt):
    """Make gregorian datetime from Jalali datetime
    >>> dt_from_jdatetime(datetime(1392, 9, 2, 23, 10, 2))
    datetime.datetime(2013, 11, 23, 23, 10, 2)
    >>> dt_from_jdatetime(datetime(1392, 6, 30, 22, 30))
    datetime.datetime(2013, 9, 21, 22, 30)
    """
    if not isinstance(jdt, datetime):
        raise TypeError
    njtm = jdt.jtm.copy()
    # to stop dst changes from bugging conversion, just convert date
    njtm.tm_hour = njtm.tm_min = njtm.tm_sec = 12
    #TODO: change after jmktime fix
    jalali_update(njtm) # pragma: jmktime needs yday
    return _std_dt_mod.datetime.fromtimestamp(jmktime(njtm)) \
            .replace(hour=jdt.hour, minute=jdt.minute, second=jdt.second,
                     microsecond=jdt.microsecond, tzinfo=jdt.tzinfo)


def jdatetime_from_jtm(jtm, microsecond=0, tz=None):
    jdt = datetime(*jtm_to_struct_time(jtm)[:6], microsecond=microsecond, tzinfo=tz)
    return jdt

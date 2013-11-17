"""
    pyjalali.time
    ~~~~~~~~~~~~~

    Time functions.

    .. Note :: Functions `jasctime`, `jctime`, `jgmtime` and `jlocaltime` not
    implemented.  Those are not thread-safe and there is no need for them in
    Python.
"""

from pyjalali import _libj
from pyjalali.types import (struct_ab_jtm, struct_jtm, struct_jyinfo, time_t,
                            time_t_p)
from ctypes import POINTER, byref, c_char_p, c_int, create_string_buffer

__all__ = ('jalali_create_date_from_days', 'jalali_create_date_from_days',
           'jalali_create_secs_from_time', 'jalali_create_time_from_secs',
           'jalali_get_date', 'jalali_get_diff', 'jalali_get_jyear_info',
           'jalali_is_jleap', '_jalali_update', 'jasctime_r', 'jctime_r',
           'jgmtime_r', 'jlocaltime_r', 'jmktime')


_jasctime_r = _libj.jasctime_r
_jasctime_r.argtypes = (POINTER(struct_jtm), c_char_p)
def jasctime_r(j_date, retain_nl=False):
    """Return string representation of jalali date from broken-down jdate
    :param date: struct_jtm
    :param retain_nl: bool
    """
    res = create_string_buffer(26)
    _jasctime_r(byref(j_date), res)
    return res.value if retain_nl else res.value[:-1]


_jctime_r = _libj.jctime_r
_jctime_r.argtypes = (time_t_p, c_char_p)
def jctime_r(timestamp, retain_nl=False):
    """Return string representation of jalali date from timestamp
    :param timestamp: int
    :param retain_nl: bool
    """
    res = create_string_buffer(26)
    _jctime_r(byref(time_t(timestamp)), res)
    return res.value if retain_nl else res.value[:-1]


_jgmtime_r = _libj.jgmtime_r
_jgmtime_r.argtypes = (time_t_p, POINTER(struct_jtm))
def jgmtime_r(timestamp):
    """Make jalali broken-down date from timestamp
    :param timestamp: int
    """
    res = struct_jtm()
    _jgmtime_r(byref(time_t(timestamp)), byref(res))
    return res


_jlocaltime_r = _libj.jlocaltime_r
_jlocaltime_r.argtypes = (time_t_p, POINTER(struct_jtm))
def jlocaltime_r(timestamp):
    """Make jalali broken-down date from timestamp according to local zone and
    dst settings
    :param timestamp: int
    """
    res = struct_jtm()
    _jlocaltime_r(byref(time_t(timestamp)), byref(res))
    return res


_jmktime = _libj.jmktime
_jmktime.argtypes = (POINTER(struct_jtm),)
_jmktime.restype = time_t
def jmktime(j_date):
    """Return timestamp from broken-down j_date according to local timezone and
    dst settings
    :param j_date: struct_jtm
    """
    return _jmktime(byref(j_date))


_jalali_is_jleap = _libj.jalali_is_jleap
_jalali_is_jleap.argtypes = (c_int,)
_jalali_is_jleap.restype = c_int
def jalali_is_jleap(year):
    return _jalali_is_jleap(year) == 1


_jalali_create_time_from_secs = _libj.jalali_create_time_from_secs
_jalali_create_time_from_secs.argtypes = (time_t, POINTER(struct_ab_jtm))
def jalali_create_time_from_secs(timestamp):
    res = struct_ab_jtm()
    _jalali_create_time_from_secs(timestamp, byref(res))
    return res


_jalali_create_secs_from_time = _libj.jalali_create_secs_from_time
_jalali_create_secs_from_time.argtypes = (POINTER(struct_ab_jtm),)
_jalali_create_secs_from_time.restype = time_t
def jalali_create_secs_from_time(ab_jtm):
    #XXX: ret sanity check
    return _jalali_create_secs_from_time(bref(ab_jtm)).value


_jalali_create_date_from_days = _libj.jalali_create_date_from_days
_jalali_create_date_from_days.argtypes = (POINTER(struct_jtm),)
def jalali_create_date_from_days(j_date, silent=False):
    res = _jalali_create_date_from_days(byref(j_date))
    if res == -1 and not silent:
        raise ValueError


_jalali_create_days_from_date = _libj.jalali_create_days_from_date
_jalali_create_days_from_date.argtypes = (POINTER(struct_jtm),)
def jalali_create_days_from_date(j_date, silent=False):
    res = _jalali_create_days_from_date(byref(j_date))
    if res == -1 and not silent:
        raise ValueError


_jalali_get_jyear_info = _libj.jalali_get_jyear_info
_jalali_get_jyear_info.argtypes = (POINTER(struct_jyinfo),)
def jalali_get_jyear_info(jyinfo):
    _jalali_get_jyear_info(byref(jyinfo))


_jalali_get_date = _libj.jalali_get_date
_jalali_get_date.argtypes = (c_int, POINTER(struct_jyinfo))
def jalali_get_date(year):
    res = struct_jyinfo()
    _jalali_get_date(year, byref(res))
    return res


_jalali_get_diff = _libj.jalali_get_diff
_jalali_get_diff.argtypes = (POINTER(struct_jtm),)
def jalali_get_diff(j_date):
    return _jalali_get_diff(byref(j_date))


_jalali_update = _libj.jalali_update
_jalali_update.argtypes = (POINTER(struct_jtm),)
def jalali_update(j_date):
    _jalali_update(byref(j_date))

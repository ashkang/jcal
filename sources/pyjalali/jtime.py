"""
    pyjalali.time
    ~~~~~~~~~~~~~

    Time functions.

    .. Note :: Functions `jasctime`, `jctime`, `jgmtime` and `jlocaltime` not
    implemented.  Those are not thread-safe and there is no need for them in
    Python.
"""

from pyjalali import _libj
from pyjalali.types import struct_jtm, time_t, time_t_p
from ctypes import POINTER, byref, c_char_p, create_string_buffer

__all__ = ('jasctime_r', 'jctime_r', 'jgmtime_r', 'jlocaltime_r', 'jmktime')


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

    Inconsistenices with POSIX jmktime:
    1. it doesn't normalize
    2. it needs glibc defined timezone information.
    3. it needs yday
    """
    return _jmktime(byref(j_date))

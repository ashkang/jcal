"""
    pyjalali.jalali
    ~~~~~~~~~~~~~~~

    libjalali custom functions.
"""

from pyjalali import _libj
from pyjalali.types import struct_ab_jtm, struct_jtm, struct_jyinfo, time_t
from ctypes import POINTER, byref, c_int

__all__ = ('jalali_create_date_from_days', 'jalali_create_date_from_days',
           'jalali_create_secs_from_time', 'jalali_create_time_from_secs',
           'jalali_get_date', 'jalali_get_diff', 'jalali_get_jyear_info',
           'jalali_is_jleap', 'jalali_update')

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
_jalali_get_date.argtypes = (c_int, POINTER(struct_jtm))
def jalali_get_date(year):
    res = struct_jtm()
    _jalali_get_date(year, byref(res))
    return res


_jalali_get_diff = _libj.jalali_get_diff
_jalali_get_diff.argtypes = (POINTER(struct_jtm),)
def jalali_get_diff(j_date):
    if _jalali_get_diff(byref(j_date)) == -1:
       raise ValueError 


_jalali_update = _libj.jalali_update
_jalali_update.argtypes = (POINTER(struct_jtm),)
def jalali_update(j_date):
    """
    this doesn't set isdst
    """
    if _jalali_update(byref(j_date)) != 0:
        raise ValueError

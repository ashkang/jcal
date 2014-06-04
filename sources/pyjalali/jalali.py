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
    """Return True if given year is leap year else False."""
    return _jalali_is_jleap(year) == 1


_jalali_create_time_from_secs = _libj.jalali_create_time_from_secs
_jalali_create_time_from_secs.argtypes = (time_t, POINTER(struct_ab_jtm))
def jalali_create_time_from_secs(timestamp):
    """Return :class:`.types.struct_ab_jtm` from given timestamp."""
    res = struct_ab_jtm()
    _jalali_create_time_from_secs(timestamp, byref(res))
    return res


_jalali_create_secs_from_time = _libj.jalali_create_secs_from_time
_jalali_create_secs_from_time.argtypes = (POINTER(struct_ab_jtm),)
_jalali_create_secs_from_time.restype = time_t
def jalali_create_secs_from_time(ab_jtm):
    """Return number of seconds elapsed since UTC Epoch based on supplied
    :class:`.types.struct_ab_jtm`.
    """
    #XXX: ret sanity check
    return _jalali_create_secs_from_time(bref(ab_jtm)).value


_jalali_create_date_from_days = _libj.jalali_create_date_from_days
_jalali_create_date_from_days.argtypes = (POINTER(struct_jtm),)
def jalali_create_date_from_days(jtm, silent=False):
    """Alter provided :class:`.types.struct_jtm` object's fields
    :attr:`~.types.struct_jtm.tm_mon` and :attr:`~.types.struct_jtm.tm_mday`
    based on its :attr:`~.types.struct_jtm.tm_yday` field.  In case of
    failure raise `ValueError` exception if silent is not True.
    """
    res = _jalali_create_date_from_days(byref(jtm))
    if res == -1 and not silent:
        raise ValueError


_jalali_create_days_from_date = _libj.jalali_create_days_from_date
_jalali_create_days_from_date.argtypes = (POINTER(struct_jtm),)
def jalali_create_days_from_date(jtm, silent=False):
    """Alter provided :class:`.types.struct_jtm` object's field
    :attr:`~.types.struct_jtm.tm_yday` based on its fields
    :attr:`~.types.struct_jtm.tm_mon` and :attr:`~.types.struct_jtm.tm_mday`.
    In case of failure raise `ValueError` exception if silent is not True.
    """
    res = _jalali_create_days_from_date(byref(jtm))
    if res == -1 and not silent:
        raise ValueError


_jalali_get_jyear_info = _libj.jalali_get_jyear_info
_jalali_get_jyear_info.argtypes = (POINTER(struct_jyinfo),)
def jalali_get_jyear_info(jyinfo):
    """Fill given :class:`.types.struct_jyinfo` object's fields with year
    information based on given year by :attr:`.types.struct_jyinfo.y`.
    """
    _jalali_get_jyear_info(byref(jyinfo))


_jalali_get_date = _libj.jalali_get_date
_jalali_get_date.argtypes = (c_int, POINTER(struct_jtm))
def jalali_get_date(days):
    """Calculates Jalali date based on given number of days since UTC
    Epoch and return result as :class:`.types.struct_jtm`.
    """
    res = struct_jtm()
    _jalali_get_date(days, byref(res))
    return res


_jalali_get_diff = _libj.jalali_get_diff
_jalali_get_diff.argtypes = (POINTER(struct_jtm),)
def jalali_get_diff(jtm, silent=False):
    """Return number of days passed since UTC Epoch based on given
    :class:`.types.struct_jtm`.  In case of failure raise `ValueError`
    exception if silent is not True.
    """
    res = _jalali_get_diff(byref(jtm))
    if res == -1 and not silent:
        raise ValueError
    return res


_jalali_update = _libj.jalali_update
_jalali_update.argtypes = (POINTER(struct_jtm),)
def jalali_update(jtm):
    """Updates given :class:`.types.struct_jtm` object's fields based on its
    :attr:`~.types.struct_jtm.tm_year`, :attr:`~.types.struct_jtm.tm_mon` and
    :attr:`~.types.struct_jtm.tm_mday`.
    """
    _jalali_update(byref(jtm))

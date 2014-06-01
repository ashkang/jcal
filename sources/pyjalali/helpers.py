"""
    pyjalali.helpers
    ~~~~~~~~~~~~~~~~

    >>> _normalized_date (1391, 11, 30) # leap year
    (1391, 11, 30)
    >>> _normalized_date (1390, 12, 1)
    (1391, 0, 1)
    >>> _normalized_date (1390, 11, 30)
    (1391, 0, 1)
    >>> _normalized_date (1390, 0, 1)
    (1390, 0, 1)
    >>> _normalized_date (1390, 0, 0)
    (1389, 11, 29)
    >>> _normalized_date (1391, 0, 0) # leap year, one day before
    (1390, 11, 29)
    >>> _normalized_date (1392, 0, 0)
    (1391, 11, 30)
    >>> _normalized_date (1392, 11, -32)
    (1392, 9, 28)
    >>> _normalized_date (1389, 11, 130)
    (1390, 3, 8)
    >>> _normalized_date (1388, 4, 32)
    (1388, 5, 1)
    >>> _normalized_date(1391, 10, 61+365)
    (1393, 0, 1)
    >>> _normalized_date(1390, 10, 365+60)
    (1391, 11, 30)
"""

from pyjalali.jalali import jalali_is_jleap, jalali_update
from pyjalali.types import struct_jtm

days_in_month = (31, 31, 31, 31, 31, 31, 30, 30, 30, 30, 30, 29)


def normalized_pair(hi, lo, factor):
    assert factor > 0
    if lo < 0 or lo >= factor:
        hix, lo = divmod(lo, factor)
        hi += hix
    return hi, lo


def _normalized_date(year, mon, mday):
    """For internal testing only.  For actual normalization call jalali_update
    or normalize_jtm.
    0 <= mon < 12
    1 <= mday < 32
    """
    jtm = struct_jtm()
    jtm.tm_year = year
    jtm.tm_mon = mon
    jtm.tm_mday = mday
    jalali_update(jtm)
    return jtm.tm_year, jtm.tm_mon, jtm.tm_mday


def normalize_jtm(jtm, microsecond=0):
    jtm.tm_sec, microsecond = normalized_pair(jtm.tm_sec, microsecond, 1000000)
    jalali_update(jtm)
    return microsecond

"""
    pyjalali.helpers
    ~~~~~~~~~~~~~~~~

    >>> normalized_date_time(1387, 10, 15, 20, 59, 58, 2000100)
    (1387, 10, 15, 21, 0, 0, 100)
    >>> normalized_date (1391, 11, 30) # leap year
    (1391, 11, 30)
    >>> normalized_date (1390, 12, 1)
    (1391, 0, 1)
    >>> normalized_date (1390, 11, 30)
    (1391, 0, 1)
    >>> normalized_date (1390, 0, 1)
    (1390, 0, 1)
    >>> normalized_date (1390, 0, 0)
    (1389, 11, 29)
    >>> normalized_date (1391, 0, 0) # leap year, one day befor
    (1390, 11, 29)
    >>> normalized_date (1392, 0, 0)
    (1391, 11, 30)
    >>> normalized_date (1392, 11, -32)
    (1392, 9, 28)
    >>> normalized_date (1389, 11, 130)
    (1390, 3, 8)
    >>> normalized_date (1388, 4, 32)
    (1388, 5, 1)

"""

from pyjalali.jalali import jalali_is_jleap

days_in_month = (31, 31, 31, 31, 31, 31, 30, 30, 30, 30, 30, 29)


def normalized_pair(hi, lo, factor):
    assert factor > 0
    if lo < 0 or lo >= factor:
        hix, lo = divmod(lo, factor)
        hi += hix
    return hi, lo


def month_days(year, mon):
    mon %= 12
    dim = days_in_month[mon]
    if mon == 11 and jalali_is_jleap(year):
        dim += 1
    return dim


def normalized_date(year, mon, mday):
    # this usage should be integrated in jmktime probably
    # TODO
    if mon < 0 or mon > 11:
        year, mon = normalized_pair(year, mon, 12)
    assert 0 <= mon <= 11

    dim = month_days(year, mon)
    if mday > dim:
        while mday > dim:
            mday -= dim
            mon += 1
            dim = month_days(year, mon)
        return normalized_date(year, mon, mday)
    del dim
    while mday < 1:
        # break month to day
        if mon == 0:
            year -= 1
            mon += 11
        else:
            mon -= 1
        mday += month_days(year, mon)

    assert 0 <= mon <= 11
    assert 1 <= mday <= month_days(year, mon)
    return year, mon, mday


def normalized_date_time(year, mon, mday, hour, min, sec, ms):
    """
    0 <= mon <= 11
    1 <= mday <= 31
    """
    sec, ms = normalized_pair(sec, ms, 1000000)
    min, sec = normalized_pair(min, sec, 60)
    hour, min = normalized_pair(hour, min, 60)
    mday, hour = normalized_pair(mday, hour, 24)
    return normalized_date(year, mon, mday) + (hour, min, sec, ms)


def normalize_jtm(jtm, microsecond=0):
    (jtm.tm_year, jtm.tm_mon, jtm.tm_mday, jtm.tm_hour,
        jtm.tm_min, jtm.tm_sec, microsecond) = normalized_date_time(
            jtm.tm_year, jtm.tm_mon, jtm.tm_mday, jtm.tm_hour,
            jtm.tm_min, jtm.tm_sec, microsecond)
    return microsecond

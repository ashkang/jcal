"""
    pyjalali.jstr
    ~~~~~~~~~~~~~

    String formatting and deformatting.
"""

from pyjalali import _libj
from pyjalali.types import struct_jtm
from ctypes import POINTER, byref
from ctypes import c_char_p, c_int, create_string_buffer

__all__ = ['jstrftime', 'jstrptime']

_jstrptime = _libj.jstrptime
_jstrptime.argtypes = (c_char_p, c_char_p, POINTER(struct_jtm))
_jstrptime.restype = c_char_p
def jstrptime(format, date_str):
    """Return date from date_str according to format
    :param date_str: string
    :param format: string
    """
    date = struct_jtm()
    res = c_char_p()
    res = _jstrptime(date_str, format, byref(date))
    return date, res


_jstrftime = _libj.jstrftime
_jstrftime.argtypes = (c_char_p, c_int, c_char_p, POINTER(struct_jtm))
def jstrftime(format, j_date):
    """Return string representation of date according to format
    :param date: struct_jtm
    :param format: string
    """
    n = len(format) << 8
    res = create_string_buffer(n)
    _jstrftime(res, n, format, byref(j_date))
    return res.value

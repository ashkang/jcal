"""
    pyjalali.jstr
    ~~~~~~~~~~~~~

    String formatting and deformatting
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
    """Return :class:`.types.struct_jtm` from date_str according to format.

    :param string format: format of string representation
    :param string date_str: string representation
    """
    date = struct_jtm()
    res = c_char_p()
    res = _jstrptime(date_str, format, byref(date))
    return date, res


_jstrftime = _libj.jstrftime
_jstrftime.argtypes = (c_char_p, c_int, c_char_p, POINTER(struct_jtm))
def jstrftime(format, jtm):
    """Return string representation of given time according to format.

    :param string format: format of date representation
    :param `pyjalali.types.struct_jtm` jtm: broken-down jalali time
    """
    n = len(format) << 8
    res = create_string_buffer(n)
    _jstrftime(res, n, format, byref(jtm))
    return res.value

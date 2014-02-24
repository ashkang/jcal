"""
    pyjalali.time
    ~~~~~~~~~~~~~

    Time functions.

    Functions `jasctime`, `jctime`, `jgmtime` and `jlocaltime` are forwarded
    to reentrant backends.
"""

from pyjalali import _libj
from pyjalali.types import struct_jtm, time_t, time_t_p
from ctypes import POINTER, byref, c_char_p, create_string_buffer

__all__ = ('jasctime', 'jctime', 'jgmtime', 'jlocaltime', 'jmktime')


_jasctime_r = _libj.jasctime_r
_jasctime_r.argtypes = (POINTER(struct_jtm), c_char_p)
def jasctime(jtm, retain_nl=False):
    """Return string representation of given time.

    :param `pyjalali.types.struct_jtm` jtm:
    :param bool retain_nl: keep trailing newline character
    """
    res = create_string_buffer(26)
    _jasctime_r(byref(jtm), res)
    return res.value if retain_nl else res.value[:-1]


_jctime_r = _libj.jctime_r
_jctime_r.argtypes = (time_t_p, c_char_p)
def jctime(timestamp, retain_nl=False):
    """Return string representation of time from timestamp.

    :param int timestamp:
    :param bool retain_nl: keep trailing newline character
    """
    res = create_string_buffer(26)
    _jctime_r(byref(time_t(timestamp)), res)
    return res.value if retain_nl else res.value[:-1]


_jgmtime_r = _libj.jgmtime_r
_jgmtime_r.argtypes = (time_t_p, POINTER(struct_jtm))
def jgmtime(timestamp):
    """Return :class:`.types.struct_jtm` from `timestamp` expressed in UTC.
    """
    res = struct_jtm()
    _jgmtime_r(byref(time_t(timestamp)), byref(res))
    return res


_jlocaltime_r = _libj.jlocaltime_r
_jlocaltime_r.argtypes = (time_t_p, POINTER(struct_jtm))
def jlocaltime(timestamp):
    """Make :class:`.types.struct_jtm` from `timestamp` according to local
    zone and dst settings.
    """
    res = struct_jtm()
    _jlocaltime_r(byref(time_t(timestamp)), byref(res))
    return res


_jmktime = _libj.jmktime
_jmktime.argtypes = (POINTER(struct_jtm),)
_jmktime.restype = time_t
def jmktime(jtm):
    """Return timestamp from provided time.

    :param `pyjalali.types.struct_jtm` jtm:

    .. Warning ::
        Inconsistenices with POSIX :c:func:`mktime`:

        1. it doesn't normalize input.
        2. it needs glibc defined timezone information.
        3. it needs :attr:`.types.struct_jtm.tm_yday` provided.
        4. it reads zone information from provided time structure instead of
           local timezone.
    """
    return _jmktime(byref(jtm))

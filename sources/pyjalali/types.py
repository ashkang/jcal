"""
    pyjalali.types
    ~~~~~~~~~~~~~~

    Core C types for libjalali binding.
"""

from ctypes import POINTER, Structure, c_char_p, c_int, c_long
from time import struct_time

class struct_ab_jtm(Structure):
    """Time passed since UTC Epoch"""
    _fields_ = (('ab_sec', c_int),
                ('ab_min', c_int),
                ('ab_hour', c_int),
                ('ab_days', c_int))

    def __str__(self):
        return '%d %d-%d%d' % (self.ab_days, self.ab_hour, self.ab_min,
                               self.ab_sec)


class struct_jtm(Structure):
    """Broken-down jalali date and time"""
    _fields_ = (('tm_sec', c_int),
                ('tm_min', c_int),
                ('tm_hour', c_int),
                ('tm_mday', c_int),
                ('tm_mon', c_int),
                ('tm_year', c_int),
                ('tm_wday', c_int),
                ('tm_yday', c_int),
                ('tm_isdst', c_int),
                ('tm_gmtoff', c_long),
                ('tm_zone', c_char_p))

    def __str__(self):
        return '%d/%d/%d %d-%d-%d +%d (%s)' % (self.tm_year, self.tm_mon,
                                              self.tm_mday, self.tm_hour,
                                              self.tm_min, self.tm_sec,
                                              self.tm_gmtoff, self.tm_zone)
    def copy(self):
        """New instance of this object"""
        return struct_jtm(self.tm_sec, self.tm_min, self.tm_hour, self.tm_mday,
                          self.tm_mon, self.tm_year, self.tm_wday,
                          self.tm_yday, self.tm_isdst, self.tm_gmtoff,
                          self.tm_zone)


class struct_jyinfo(Structure):
    """Year specific information"""
    _fields_ = (('lf', c_int),
                ('y', c_int),
                ('r', c_int),
                ('p', c_int),
                ('rl', c_int),
                ('pl', c_int),
                ('apl', c_int))


time_t = c_int
time_t_p = POINTER(c_int)

def jtm_to_struct_time(src_jtm):
    """Make :class:`time.struct_time` from broken-down jalali time
    structure"""
    return struct_time((src_jtm.tm_year, src_jtm.tm_mon+1, src_jtm.tm_mday,
                        src_jtm.tm_hour, src_jtm.tm_min, src_jtm.tm_sec,
                        src_jtm.tm_wday, src_jtm.tm_yday+1, src_jtm.tm_isdst))

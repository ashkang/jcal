"""
    pyjalali
    ~~~~~~~~

    Python bindings for libjalali.

    Low level API could be accessed through :data:`jstr`, :data:`jtime` and
    :data:`jalali` modules.  Core libjalali data structures resides in
    :data:`types` module.
    
    An implementation of standard :data:`datetime`'s :class:`date` and
    :class:`datetime` provided in module :data:`~pyjalali.datetime` using
    libjalali tools.
"""

import sys
import ctypes.util
from ctypes import cdll

# hardcoded libjalali version, binding revision
__version__ = (0, 5, 0, 1)

if sys.platform.startswith('win'):
    libname = 'libjalali.dll'
else:
    libname = ctypes.util.find_library('jalali')
_libj = cdll.LoadLibrary(libname)
del cdll, ctypes, sys, libname

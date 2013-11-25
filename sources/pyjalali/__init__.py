"""
    pyjalali
    ~~~~~~~~

    Python bindings for libjalali.

    Low level API could be accessed through :data:`~pyjalali.jstr` and
    :data:`~pyjalali.ctime` modules.  Core libjalali data structures resides
    in `types` module.
    
    An implementation of standard datetime module provided in module datetime
    using libjalali tools.
"""

import sys
import ctypes.util
from ctypes import cdll

if sys.platform.startswith('win'):
    libname = 'libjalali.dll'
else:
    libname = ctypes.util.find_library('jalali')
_libj = cdll.LoadLibrary(libname)
del cdll, ctypes, sys, libname

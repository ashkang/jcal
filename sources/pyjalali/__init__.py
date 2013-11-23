"""
    pyjalali
    ~~~~~~~~

    Python bindings for libjalali.

    Low level API could be accessed through :data:`~pyjalali.jstr` and
    :data:`~pyjalali.ctime` modules.

    libjalali data structures resides in `types`
"""


from ctypes import cdll
_libj = cdll.LoadLibrary('libjalali.so')

import pyjalali.jtime, pyjalali.jstr, pyjalali.jalali

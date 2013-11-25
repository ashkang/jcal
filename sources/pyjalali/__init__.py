"""
    pyjalali
    ~~~~~~~~

    Python bindings for libjalali.

    Low level API could be accessed through :data:`~pyjalali.jstr` and
    :data:`~pyjalali.ctime` modules.  core libjalali data structures resides
    in `types
    
    An implementation of standard datetime module provided in module datetime
    using libjalali tools.
    """


from ctypes import cdll
_libj = cdll.LoadLibrary('libjalali.so')

import pyjalali.jtime, pyjalali.jstr, pyjalali.jalali, pyjalali.datetime

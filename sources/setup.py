#!/bin/env python

import os
from distutils.core import setup

try:
    src_p = os.path.dirname(os.path.abspath(__file__))
    mkfile_p = os.path.join(src_p, 'libjalali', 'Makefile.am')
    with open(mkfile_p) as mkfile:
        content = mkfile.read()
        snt = '-version-info'
        pos = content.find(snt)
        assert pos > -1
        vpos = pos + len(snt) + 1
        __version__ = content[vpos:].split('\n', 1)[0].replace(':','.')
except:
    __version__ = 'unknown'

setup(
        name='pyjalali',
        description='Jalali calendar tools based on libjalali',
        url='http://github.com/ashkang/jcal',
        version=__version__,
        packages=['pyjalali']
)

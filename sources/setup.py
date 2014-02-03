#!/bin/env python

import os
import sys
from distutils.core import setup
src_p = os.path.dirname(os.path.abspath(__file__))
sys.path.insert(0, src_p)
import pyjalali

version = '.'.join(map(str, pyjalali.__version__))

setup(
        name='pyjalali',
        description='Jalali calendar tools based on libjalali',
        long_description=pyjalali.__doc__.replace(' '*4, ''),
        url='http://github.com/ashkang/jcal',
        license='GPLv3',
        version=version,

        packages=['pyjalali'],

        classifiers = [
            'Development Status :: 3 - Alpha',
            'Intended Audience :: Developers',
            'License :: OSI Approved :: GNU General Public License v3 (GPLv3)',
            'Topic :: Software Development :: Libraries :: Python Modules',
            'Topic :: Software Development :: Localization'
        ]
)

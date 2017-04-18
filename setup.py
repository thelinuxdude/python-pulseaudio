#!/usr/bin/env python

from distutils.core import setup

setup(name='libpulseaudio',
      version='9.0',
      description='simple libpulseaudio bindings',
      author='Valodim',
      author_email='valodim@mugenguild.com',
      license='LGPL',
      url='http://github.com/thelinuxdude/python-pulseaudio',
      packages=['pulseaudio'],
      provides=['libpulseaudio']
     )

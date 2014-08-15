#!/usr/bin/env python

from distutils.core import setup

setup(name='libpulseaudio',
      version='5.0',
      description='simple libpulseaudio bindings',
      author='Valodim',
      author_email='valodim@mugenguild.com',
      license='LGPL',
      url='http://github.com/valodim/python-pulseaudio',
      packages=['pulseaudio'],
      provides=['libpulseaudio'],
      download_url='https://github.com/Valodim/python-pulseaudio/archive/pa-5.0.zip'
     )

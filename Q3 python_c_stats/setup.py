# setup.py - Build script for the stats_module C extension.
# Uses setuptools to compile stats_module.c and register the functions with Python.

from setuptools import setup, Extension

module = Extension('stats_module', sources=['stats_module.c'])

setup(
    name='stats_module',
    version='1.0',
    description='Statistics module in C',
    ext_modules=[module]
)

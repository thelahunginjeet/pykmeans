from distutils.core import setup,Command
from numpy.distutils.misc_util import get_numpy_include_dirs
from distutils.extension import Extension
import os,sys

if sys.platform == 'darwin':
    from distutils import sysconfig
    vars = sysconfig.get_config_vars()
    vars['LDSHARED'] = vars['LDSHARED'].replace('-bundle','-dynamiclib')

libkmeans = Extension("libkmeans",
                    sources = ["kmeans.c","pcg_basic.c"])

setup(
    name = 'pykmeans',
    packages = ['pykmeans'],
    package_dir = {'pykmeans': ''},
    ext_package = 'pykmeans',
    ext_modules = [libkmeans]
)

from distutils.core import setup,Command
from numpy.distutils.misc_util import get_numpy_include_dirs
from distutils.extension import Extension
import os,sys,sysconfig 

_CLUSTER = True

extra_compile_args = sysconfig.get_config_var('CFLAGS').split()
if _CLUSTER:
   extra_compile_args += ["-std=c99"]

if sys.platform == 'darwin':
    from distutils import sysconfig
    vars = sysconfig.get_config_vars()
    vars['LDSHARED'] = vars['LDSHARED'].replace('-bundle','-dynamiclib')

libkmeans = Extension("libkmeans",
                    sources = ["kmeans.c","pcg_basic.c"],
                    extra_compile_args=extra_compile_args)

setup(
    name = 'pykmeans',
    version = '1.0.0',
    packages = ['pykmeans'],
    package_dir = {'pykmeans': ''},
    ext_package = 'pykmeans',
    ext_modules = [libkmeans]
)

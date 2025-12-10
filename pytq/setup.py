from setuptools import setup, Extension
from setuptools.command.build_ext import build_ext
import sys
import os
import subprocess

class CMakeExtension(Extension):
    def __init__(self, name, sourcedir=''):
        Extension.__init__(self, name, sources=[])
        self.sourcedir = os.path.abspath(sourcedir)

class CMakeBuild(build_ext):
    def run(self):
        try:
            subprocess.check_output(['cmake', '--version'])
        except OSError:
            raise RuntimeError("CMake must be installed to build pytq")
        
        for ext in self.extensions:
            self.build_extension(ext)
    
    def build_extension(self, ext):
        extdir = os.path.abspath(os.path.dirname(self.get_ext_fullpath(ext.name)))
        
        cmake_args = [
            f'-DCMAKE_LIBRARY_OUTPUT_DIRECTORY={extdir}',
            f'-DPYTHON_EXECUTABLE={sys.executable}',
            '-DCMAKE_BUILD_TYPE=Release'
        ]
        
        build_args = ['--config', 'Release']
        
        if not os.path.exists(self.build_temp):
            os.makedirs(self.build_temp)
        
        # Build tq-core first if needed
        core_dir = os.path.join(ext.sourcedir, '..', 'tq-core')
        core_build = os.path.join(core_dir, 'build')
        
        if not os.path.exists(core_build):
            os.makedirs(core_build)
            subprocess.check_call(['cmake', '..'], cwd=core_build)
            subprocess.check_call(['cmake', '--build', '.', '--config', 'Release'], cwd=core_build)
        
        # Build Python extension
        subprocess.check_call(['cmake', ext.sourcedir] + cmake_args, cwd=self.build_temp)
        subprocess.check_call(['cmake', '--build', '.'] + build_args, cwd=self.build_temp)

setup(
    name='pytq',
    version='1.0.0',
    author='TQ Project',
    description='Python bindings for TQ query engine',
    long_description=open('README.md').read() if os.path.exists('README.md') else '',
    long_description_content_type='text/markdown',
    ext_modules=[CMakeExtension('pytq')],
    cmdclass=dict(build_ext=CMakeBuild),
    zip_safe=False,
    python_requires='>=3.7',
)

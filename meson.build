# Copyright (C) 2025 Rob Hall
# SPDX-License-Identifier: MIT

project('libdvdread', 'c',
    version: '7.0.0',
    meson_version: '>= 0.60.0',
    default_options: ['buildtype=debugoptimized',
                      'c_std=c11',
                      'default_library=both',
                      'warning_level=2'])

dvdread_src_root = meson.current_source_dir()
dvdread_build_root = meson.current_build_dir()
cc = meson.get_compiler('c')

# Configuration data for config.h
cdata = configuration_data()

# Include directories
dvdread_inc_dirs = include_directories('.', 'src', 'src/dvdread')
if cc.get_id() == 'msvc' or cc.get_id() == 'clang-cl'
    # extra POSIX headers not found in the Windows SDK
    dvdread_inc_dirs = [dvdread_inc_dirs, include_directories('msvc/include', 'msvc/contrib/dirent')]
endif

# The version number for the shared library
dvdread_soname_version = '8.1.0'

dvdread_version = meson.project_version()
dvdread_version_split = dvdread_version.split('.')
cdata.set_quoted('PACKAGE_VERSION', dvdread_version)

#
# OS/Compiler checks and defines
#

# Arguments in test_args will be used even on feature tests
test_args = ['-D_DEFAULT_SOURCE']

optional_arguments = []
optional_link_arguments = []
extra_dependencies = []

if host_machine.system() == 'sunos'
    test_args += '-D__EXTENSIONS__'
endif

# Header checks

check_headers = [
    'sys/param.h',
    'limits.h',
]

foreach h : check_headers
    if cc.has_header(h, args: test_args)
        cdata.set('HAVE_' + h.underscorify().to_upper(), 1)
    endif
endforeach

cdata.set('UNUSED', cc.has_function_attribute('unused') ? '__attribute__((unused))' : '')

if host_machine.endian() == 'big'
  cdata.set('WORDS_BIGENDIAN', 1)
endif

if host_machine.system() == 'windows'
# dvdread_headers += files('msvc/contrib/win32_cs.h')
    if cc.has_function('gettimeofday', prefix: '#include <sys/time.h>', args: test_args)
        cdata.set('HAVE_GETTIMEOFDAY', 1)
    endif
elif host_machine.system() == 'linux'
    if cc.has_function('getmntent_r', prefix: '#include <mntent.h>', args: test_args)
        cdata.set('HAVE_GETMNTENT_R', 1)
    endif
endif

if cc.has_function('strerror_r', prefix: '#include <string.h>', args: test_args)
    cdata.set('HAVE_STRERROR_R', 1)
    cdata.set('HAVE_DECL_STRERROR_R', 1)
elif cc.has_function('strerror_s', prefix: '#include <string.h>', args: test_args)
    cdata.set('HAVE_STRERROR_S', 1)
endif

cdata.set10('HAVE_STATIC_ASSERT', cc.compiles('_Static_assert(1, "The impossible happened.");', name : '_Static_assert'))

# External dependencies and libraries

# libdvdcss is loaded dynamically if it is not found or requested
libdl_dependency = []
libdvdcss_dependency = dependency('libdvdcss',
    version: '>= 1.2', method: 'pkg-config', required: get_option('libdvdcss'))
if libdvdcss_dependency.found()
    cdata.set('HAVE_DVDCSS_DVDCSS_H', 1)
elif host_machine.system() != 'windows'
    # Using builtin dlfcn (may require linking to libdl depending on the system)
    if not cc.has_function('dlopen', prefix: '#include <dlfcn.h>', args: test_args)
        libdl_dependency = cc.find_library('dl')
    endif
    cdata.set('HAVE_DLFCN_H', 1)
endif

# Compiler flag tests

# Compiler flags that should be set
# But when the compiler does not supports them
# it is not an error and silently tolerated

if get_option('warning_level') in ['2', '3', 'everything']
    optional_arguments += '-Wsign-compare'
endif

add_project_arguments(cc.get_supported_arguments(optional_arguments), language: 'c')
add_project_link_arguments(cc.get_supported_link_arguments(optional_link_arguments), language: 'c')
add_project_arguments(test_args, language: 'c')

# Generate config.h
config_h_target = configure_file(output: 'config.h', configuration: cdata)

install_data(['AUTHORS', 'COPYING', 'NEWS', 'README.md', 'TODO'],
    install_dir: get_option('datadir') / 'doc/libdvdread',
    install_tag: 'doc')

if import('fs').is_file('.git/logs/HEAD')
    git_exe = find_program('git', required: false, disabler: true)
    custom_target('ChangeLog', output: 'ChangeLog',
        depend_files: '.git/logs/HEAD',
        command: ['git', 'log'], capture: true,
        install: true,
        install_dir: get_option('datadir') / 'doc/libdvdread',
        install_tag: 'doc')
endif

# Include subdir meson.build files
# The order is important!

subdir('src/dvdread')

subdir('src')

subdir('doc')


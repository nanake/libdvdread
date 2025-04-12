# Copyright (C) 2025 Rob Hall
# SPDX-License-Identifier: MIT

if not get_option('enable_docs')
    subdir_done()
endif

doxygen = find_program('doxygen', required: true)

doxyfile_conf = configuration_data()
doxyfile_conf.set('INPUT', f'@dvdread_src_root@/src/dvdread \\\n @dvdread_build_root@/src/dvdread')
doxyfile_conf.set('PROJECT_NAME', 'libdvdread')
doxyfile_conf.set('STRIP_FROM_PATH', f'@dvdread_src_root@/src \\\n @dvdread_build_root@/src')
doxyfile_conf.set('OUTPUT_DIRECTORY', meson.current_build_dir())
doxyfile_conf.set('INPUT_DIRECTORY', meson.current_source_dir())
doxyfile_conf.set('PROJECT_VERSION', meson.project_version())

doxyfile = configure_file(
    input: 'doxygen.cfg.in',
    output: 'Doxyfile',
    configuration: doxyfile_conf,
)

custom_target('docs',
    depend_files: dvdread_api_headers,
    command: [doxygen, '-q', doxyfile],
    output: ['html'],
    install: true,
    install_dir: get_option('datadir') / 'doc/libdvdread',
    install_tag: 'doc',
)

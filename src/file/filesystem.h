/*
 * This file is part of libdvdread.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 2.1 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston MA 02110-1301, USA.
 */

#ifndef LIBDVDREAD_FILESYSTEM_H
#define LIBDVDREAD_FILESYSTEM_H

#include "dvdread/dvd_filesystem.h"

/**
 * Prototype definition for default file open function (implemented by each platform)
 */
dvd_file_h* file_open_default(dvd_reader_filesystem_h *fs, const char* filename);

/**
 * Prototype definition for default dir open function (implemented by each platform)
 */
dvd_dir_h* dir_open_default(dvd_reader_filesystem_h *fs, const char* dirname);

/**
 * Prototype definition for default stat function (implemented by each platform)
 */
int stat_default(dvd_reader_filesystem_h *fs, const char *path, dvdstat_t* statbuf);

/**
 * Inits the internal (platform specific) filesystem implementation
 * bundled with libdvdread. This includes initializing the default internal
 * implmentations of file_open, dir_open, stat, etc.
 */
dvd_reader_filesystem_h* InitInternalFilesystem();

#endif

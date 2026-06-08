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

/* default filesystem operations, each platform provides its own */
void   *file_open_default(dvd_reader_filesystem_h *fs, const char* filename);
ssize_t file_read_default(void *file, char *buf, size_t size);
off64_t file_seek_default(void *file, off64_t offset, int whence);
int     file_close_default(void *file);

void   *dir_open_default(dvd_reader_filesystem_h *fs, const char* dirname);
int     dir_read_default(void *dir, dvd_dirent_t *entry);
void    dir_close_default(void *dir);

int     stat_default(dvd_reader_filesystem_h *fs, const char *path, dvdstat_t* statbuf);

/* set up the internal filesystem bundled with libdvdread */
dvd_reader_filesystem_h* InitInternalFilesystem(void);

#endif

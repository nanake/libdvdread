/*
 * This file is part of libdvdread
 * Copyright (C) 2022 VideoLAN
 *
 * This file is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This file is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library. If not, see
 * <http://www.gnu.org/licenses/>.
 */

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>

#include <sys/stat.h>
#include <fcntl.h>

#include <windows.h>
#include "../msvc/contrib/win32_cs.h"

#include <dvdread/dvd_filesystem.h>
#include "filesystem.h"


void *file_open_default(dvd_reader_filesystem_h *fs, const char* filename)
{
    if (!fs)
        return NULL;

    int *handle;
    int fd;
    wchar_t *wpath;

    wpath = _utf8_to_wchar(filename);
    if (!wpath) {
        return NULL;
    }

    fd = _wopen(wpath, O_RDONLY | O_BINARY);
    free(wpath);
    if (fd < 0) {
        return NULL;
    }

    handle = malloc(sizeof(*handle));
    if (!handle) {
        close(fd);
        return NULL;
    }
    *handle = fd;

    return handle;
}

ssize_t file_read_default(void *file, char *buf, size_t size)
{
    if (size == 0) {
        return 0;
    }

    return read(*(int*)file, buf, size);
}

off64_t file_seek_default(void *file, off64_t offset, int whence)
{
    return _lseeki64(*(int*)file, offset, whence);
}

int file_close_default(void *file)
{
    if (file) {
        int ret = close(*(int*)file);
        free(file);
        return ret;
    }
    return 0;
}

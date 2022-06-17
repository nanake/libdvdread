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

#include <dvdread/dvd_filesystem.h>

#include "filesystem.h"

#ifdef __ANDROID__
# undef  lseek
# define lseek lseek64
# undef  off_t
# define off_t off64_t
#endif


int _file_close(dvd_file_h *file)
{
    if (file) {
        int ret = close((int)(intptr_t)file->internal);
        free(file);
        return ret;
    }
    return 0;
}

int64_t _file_seek(dvd_file_h *file, int64_t offset, int32_t origin)
{
    off_t result = lseek((int)(intptr_t)file->internal, offset, origin);
    if (result == (off_t)-1) {
        return -1;
    }
    return (int64_t)result;
}

ssize_t _file_read(dvd_file_h *file, char *buf, size_t size)
{
    ssize_t result;

    if (size <= 0) {
        return 0;
    }

    result = read((int)(intptr_t)file->internal, buf, size);
    return result;
}


dvd_file_h* file_open_default(dvd_reader_filesystem_h *fs, const char* filename)
{
    if (!fs)
        return NULL;

    dvd_file_h *file;
    int fd    = -1;
    int flags = 0;
    int mode  = 0;

    #if defined(__OS2__) // not posix but kept here for legacy compatibility reasons
    flags = O_RDONLY | O_BINARY;
    #else
    flags = O_RDONLY;
    #endif

#ifdef O_CLOEXEC
    flags |= O_CLOEXEC;
#endif
#ifdef O_BINARY
    flags |= O_BINARY;
#endif

    if ((fd = open(filename, flags, mode)) < 0) {
        return NULL;
    }

    file = calloc(1, sizeof(dvd_file_h));
    if (!file) {
        close(fd);
        return NULL;
    }

    file->close = _file_close;
    file->read = _file_read;
    file->seek  = _file_seek;
    file->internal = (void*)(intptr_t)fd;

    return file;
}

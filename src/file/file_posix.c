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

#if defined(__APPLE__) || defined(__FreeBSD__) || defined(__OpenBSD__) || \
    defined(__NetBSD__) || defined(__DragonFly__) || \
    (defined(_FILE_OFFSET_BITS) && _FILE_OFFSET_BITS >= 64)
/* off_t is already 64 bit; lseek handles off64_t offsets */
#else
# undef  lseek
# define lseek lseek64
# undef  off_t
# define off_t off64_t
#endif


void *file_open_default(dvd_reader_filesystem_h *fs, const char* filename)
{
    if (!fs)
        return NULL;

    int *handle;
    int fd;
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
    return lseek(*(int*)file, offset, whence);
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

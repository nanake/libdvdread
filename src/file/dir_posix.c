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

#include "config.h"

#include <stdlib.h>
#include <string.h>
#if HAVE_DIRENT_H
#include <dirent.h>
#endif

#if defined(__GLIBC__) && defined(__GLIBC_MINOR__)
#  if __GLIBC__ > 2 || (__GLIBC__ == 2 && __GLIBC_MINOR__ >= 24)
#    define USE_READDIR
#    include <errno.h>
#  endif
#endif

#include <dvdread/dvd_filesystem.h>
#include "filesystem.h"

void *dir_open_default(dvd_reader_filesystem_h *fs, const char* dirname)
{
    if (!fs)
        return NULL;

    return opendir(dirname);
}

int dir_read_default(void *dir, dvd_dirent_t *entry)
{
    struct dirent *p_e;

#ifdef USE_READDIR
    errno = 0;
    p_e = readdir((DIR*)dir);
    if (!p_e && errno) {
        return -errno;
    }
#else /* USE_READDIR */
    int result;
    struct dirent e;

    result = readdir_r((DIR*)dir, &e, &p_e);
    if (result) {
        return -result;
    }
#endif /* USE_READDIR */

    if (p_e == NULL) {
        return 1;
    }
    strncpy(entry->d_name, p_e->d_name, sizeof(entry->d_name));
    entry->d_name[sizeof(entry->d_name) - 1] = 0;

    return 0;
}

void dir_close_default(void *dir)
{
    if (dir)
        closedir((DIR *)dir);
}

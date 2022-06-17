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
#include <sys/stat.h>

#include <windows.h>
#include "../msvc/contrib/win32_cs.h"

#include <dvdread/dvd_filesystem.h>
#include "filesystem.h"


int stat_default(dvd_reader_filesystem_h *fs, const char *path, dvdstat_t* statbuf)
{
    if (!fs)
        return -1;

    struct _stat64 win32statbuf;

    wchar_t *wpath, *it;

    wpath = _utf8_to_wchar(path);
    if (!wpath) {
        return -1;
    }

    /* need to strip possible trailing \\ */
    for (it = wpath; *it; it++)
        if ((*it == '\\' || *it == '/') && *(it+1) == 0)
        *it = 0;

    int ret = _wstat64(wpath, &win32statbuf);
    statbuf->size = win32statbuf.st_size;
    statbuf->st_mode = win32statbuf.st_mode;
    return ret;
}

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

#include <io.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include <windows.h>
#include "../msvc/contrib/win32_cs.h"


#include <dvdread/dvd_filesystem.h>
#include "filesystem.h"


typedef struct {
  intptr_t handle;
  struct _wfinddata_t went;
} win32_dir_t;


void _dir_close_win32(dvd_dir_h *dir)
{
    if (dir) {
        _findclose(((win32_dir_t*)dir->internal)->handle);
        free((win32_dir_t*)dir->internal);
        free(dir);
        dir = NULL;
    }
}

int _dir_read_win32(dvd_dir_h *dir, dvd_dirent_t *entry)
{
    win32_dir_t *wdir = (win32_dir_t*)dir->internal;
    if (wdir->went.name[0]) {
        if (!WideCharToMultiByte(CP_UTF8, 0, wdir->went.name, -1, entry->d_name, sizeof(entry->d_name), NULL, NULL))
            entry->d_name[0] = 0; /* allow reading next */
        wdir->went.name[0] = 0;
        _wfindnext(wdir->handle, &wdir->went);
        return 0;
    }
    return -1;
}

dvd_dir_h *dir_open_default(dvd_reader_filesystem_h *fs, const char* dirname)
{
    if (!fs)
        return NULL;

    char    *filespec;
    wchar_t *wfilespec;
    win32_dir_t *d;
    dvd_dir_h *dir = calloc(1, sizeof(dvd_dir_h));

    if (!dir) {
        return NULL;
    }

    d = calloc(1, sizeof(*d));
    if (!d)
    {
        free(dir);
        return NULL;
    }

    filespec = malloc(strlen(dirname) + 3);
    if (!filespec) {
        goto fail;
    }
    sprintf(filespec, "%s\\*", dirname);

    wfilespec = _utf8_to_wchar(filespec);
    free(filespec);
    if (!wfilespec) {
        goto fail;
    }

    d->handle = _wfindfirst(wfilespec, &d->went);
    free(wfilespec);
    if (d->handle != -1) {
        dir->internal = (void*)d;
        dir->close = _dir_close_win32;
        dir->read = _dir_read_win32;
        return dir;
    }

    fail:
        free(d);
        free(dir);
        return NULL;
}

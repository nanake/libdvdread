/*
 * This file is part of libdvdread
 * Copyright (C) 2026 VideoLAN
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

#ifndef DVDREAD_FILESYSTEM_H_
#define DVDREAD_FILESYSTEM_H_

#include <stdint.h>
#include <sys/types.h>

/*
 * directory access
 */

/**
 * The dvd_dirent_t struct abstracts dirent usage from the respective platform implementation.
 * For this reason, it only includes the parts the lib cares about (in this case d_name - the directory name)
 */
typedef struct
{
    char    d_name[256];
} dvd_dirent_t;

typedef struct dvd_filesystem_dir_s dvd_dir_h;
/**
 * Abstraction for directory access
 * internal - opaque handler internal to the specific implementation used to store the data type of directory stream objects.
 * This is tipically a dir handler (e.g. a DIR* in posix)
 * close(dvd_dir_h *dir) - called to close the directory and cleanup any alloc'd dir structures
 * read(dvd_dir_h *dir, dvd_dirent_t *entry) - provided the dvd_dirent_t, reads the respective directory and returns -1 if the
 * directory could not be read, 0 if successfully read.
 */
struct dvd_filesystem_dir_s
{
    void *internal;
    void (*close)(dvd_dir_h *dir);
    int (*read)(dvd_dir_h *dir, dvd_dirent_t *entry);
};

/*
 * Stat access
 */

#define DVD_S_IFMT	       0170000     /* These bits determine file type.  */

#define DVD_S_IFCHR        0020000     /* character special */
#define DVD_S_IFDIR        0040000     /* directory */
#define DVD_S_IFBLK        0060000     /* block special */
#define DVD_S_IFREG        0100000     /* regular */

/**
 * Abstraction for stat buffer structure
 * size - size of the stat'd file
 * st_mode - file mode
 */
typedef struct
{
    off_t size;
    unsigned int st_mode;
} dvdstat_t;


/*
 * file access
 */

/**
 * Abstraction for file access
 * internal - opaque handler internal to the specific implementation used to store the data type of the file object.
 * This is tipically a file reference (e.g. the file descriptor/fd)
 * close(dvd_file_h *file) - called to close the file and cleanup any alloc'd file structs
 * seek(dvd_file_h *file, int64_t offset, int32_t origin) - used to seek into the given file (provided the offset and the origin). Returns
 * the position on the file after seek.
 * read(dvd_file_h *file, char *buf, int64_t size) - used to read the file into the passed buffer, given the read size. Returns the read
 * read from the file
 */
typedef struct dvd_filesystem_file_s dvd_file_h;
struct dvd_filesystem_file_s
{
    void *internal;
    int (*close) (dvd_file_h *file);
    int64_t (*seek)  (dvd_file_h *file, int64_t offset, int32_t origin);
    ssize_t (*read)  (dvd_file_h *file, char *buf, size_t size);
};

/*
 * Filesystem implementation
 */

/**
 * Groups all filesystem operations into a common struct. This is the struct external applications should override to
 * provide custom filesystem implementations:
 * internal - opaque data pointer to user data (used to convey custom data within the filesystem struct)
 * close(dvd_reader_filesystem_h *fs) - called to destroy the filesystem implementation (free any alloc'd structs)
 * stat(dvd_reader_filesystem_h *fs, const char *path, dvdstat_t* statbuf) - stat a file/dir provided the statbuf, initializes the dvdstat_t.
 * dir_open(dvd_reader_filesystem_h *fs, const char* dirname) - open the provided dir, initializes dvd_dir_h
 * file_open(dvd_reader_filesystem_h *fs, const char* filename, const char *mode) - open a file, initializes dvd_file_h
 */
typedef struct dvd_reader_filesystem_s dvd_reader_filesystem_h;
struct dvd_reader_filesystem_s
{
    void *internal;
    void (*close) (dvd_reader_filesystem_h *fs);
    int (*stat) (dvd_reader_filesystem_h *fs, const char *path, dvdstat_t *statbuf);
    dvd_dir_h* (*dir_open) (dvd_reader_filesystem_h *fs, const char *dirname);
    dvd_file_h* (*file_open) (dvd_reader_filesystem_h *fs, const char *filename);
};

#endif /* DVDREAD_FILESYSTEM_H_ */

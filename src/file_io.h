#ifndef FILE_IO_H
#define FILE_IO_H

#include <stdio.h>
#include <stdlib.h>

/*
    This file provides methods for reading and writing ascii-encoded files
*/

size_t file_size(FILE* fp);

FILE* file_open(const char* dir, const char* mode);

char* file_read(FILE* fp);

int file_write(FILE* fp, const char* data_buffer);

int file_close(FILE* fp);

#endif
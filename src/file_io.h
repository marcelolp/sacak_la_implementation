#ifndef FILE_IO_H
#define FILE_IO_H

#include <stdio.h>
#include <stdlib.h>

/*
    Provides methods for reading and writing files, interpreting the content as chars
*/

size_t file_size(FILE* fp);

FILE* file_open(const char* dir, const char* mode);

char* file_read(FILE* fp);

int file_write(FILE* fp, const char* data_buffer, size_t data_size);

int file_close(FILE* fp);

#endif
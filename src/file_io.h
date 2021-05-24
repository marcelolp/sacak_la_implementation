#ifndef FILE_IO_H
#define FILE_IO_H

#include <stdio.h>
#include <stdlib.h>

/*
    Provides methods for reading and writing files, interpreting the content as chars
*/


/*
*  Determines the size of the file reference by the filepointer fp.
*/
size_t file_size(FILE* fp);

/*
*  Opens the file dir and returns garantueed non-zero filepointer fp to it.
*/
FILE* file_open(unsigned char* dir, unsigned char* mode);

/*
*  Reads the content of the file until EOF is reached.
*  A buffer will be returned with  size = (file_size+2)*byte_size.
*  Position buffer[file_size] will be '$', position buffer[file_size + 1] will be '/0'
*/
unsigned int* file_read_d_term(FILE* fp);

/*
*  Reads the content of the file until EOF is reached.
*  A buffer will be returned with  size = (file_size+1)*byte_size.
*  position buffer[file_size] will be '/0'
*/
unsigned char* file_read(FILE* fp);

/*
*  Writes the content of data_buffer interpreted as chars into the stream fp
*/
int file_write(FILE* fp, unsigned char* data_buffer, size_t data_size);

/* 
*   Closes the file fp
*/
int file_close(FILE* fp);

#endif
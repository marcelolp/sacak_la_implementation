#include "file_io.h"

/*
*  Determines the size of the file reference by the filepointer fp.
*/
size_t file_size(FILE* fp) {
    size_t f_size = 0;
    size_t f_pos = ftell(fp);                                                                       // store file position
    if (fseek(fp, 0L, SEEK_END) != 0) {                                                             // seek EOF
        perror("fseek: ");
        exit(-1);
    }
    f_size = ftell(fp);                                                                             // file position is at the EOF
    if (fseek(fp, f_pos, SEEK_SET) != 0) {                                                          // restore file position
        perror("fseek: ");
        exit(-1);
    }
    return f_size;
}

/*
*  Opens the file dir and returns garantueed non-zero filepointer fp to it.
*/
FILE* file_open(const char* dir, const char* mode) {
    FILE* fp = fopen(dir, mode);
    if (fp == 0) {                                                                                  // check for errors
        printf("Cannot open file %s \n", dir);
    }
    return fp;
}

/*
*  Reads the content of the file until EOF is reached.
*  A buffer will be returned with  size = (file_size+2)*byte_size.
*  Position buffer[file_size] will be '$', position buffer[file_size + 1] will be '/0'
*/
char* file_read(FILE* fp) {
    size_t f_size = file_size(fp);
    
    char* buffer = (char*) malloc((f_size + 2) * sizeof(char));                                     // buffer has size f_size+1 so that the termination symbols $0 can be added
    
    size_t num_read = fread(buffer, sizeof(char), f_size, fp);
    if (num_read < f_size) {
        perror("fread: ");
        printf("Could only read %u chars", num_read);
        exit(-1);
    }
    buffer[f_size] = '$';
    buffer[f_size + 1] = '\0';
    return buffer;
}

/*
*  Writes the content of data_buffer interpreted as chars into the stream fp
*/
int file_write(FILE* fp, const char* data_buffer, size_t buffer_size) {

    size_t num_written = fwrite(data_buffer, sizeof(char), buffer_size, fp);
    if (num_written < buffer_size) {
        perror("fwrite: ");
        printf("Could only write %u chars", num_written);
        exit(-1);
    }
    return 0;
}

/* 
*   Closes the file fp
*/
int file_close(FILE* fp) {
    if (fclose(fp) != 0) {
        perror("fclose: ");
        exit(-1);
    }    
    return 0;
}
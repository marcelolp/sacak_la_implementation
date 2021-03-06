#include "file_io.h"


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


FILE* file_open( const char* dir,  const char* mode) {
    FILE* fp = fopen(dir, mode);
    if (fp == 0) {                                                                                  // check for errors
        perror("fopen: ");
        printf("Cannot open file %s \n", dir);
    }
    return fp;
}


unsigned int* file_read_d_term(FILE* fp) {
    size_t f_size = file_size(fp);
    
    unsigned char* buffer = (unsigned char*) malloc((f_size + 2) * sizeof(unsigned char));             // buffer has size f_size+2 so that the termination symbols $\0 can be added
    unsigned int* ibuffer = (unsigned int*) malloc((f_size + 2) * sizeof(unsigned int));
    
    if (buffer == 0 || ibuffer == 0) {
            perror("malloc: ");
            printf("Could not allocate memory for file input\n");
            exit(-1);
    }

    size_t num_read = fread(buffer, sizeof(unsigned char), f_size, fp);
    if (num_read < f_size) {
        perror("fread: ");
        printf("Could only read %u chars\n", num_read);
        printf("Last chars read:\n");
        for (int i = num_read >= 50 ? num_read - 50 : 0; i < num_read; i++) {
            printf("%c", buffer[i]);
        }
        exit(-1);
    }
    buffer[f_size] = '$';
    buffer[f_size + 1] = '\0';

    for (int i = 0; i < f_size+2; i++) {
        ibuffer[i] = (unsigned int) buffer[i];
    }
    free(buffer);

    return ibuffer;
}


unsigned char* file_read(FILE* fp) {
    size_t f_size = file_size(fp);
    
    unsigned char* buffer = (unsigned char*) malloc((f_size + 1) * sizeof(unsigned char));          // buffer has size f_size+1 so that the termination symbols $\0 can be added
    
    size_t num_read = fread(buffer, sizeof(unsigned char), f_size, fp);
    if (num_read < f_size) {
        perror("fread: ");
        printf("Could only read %u chars", num_read);
        exit(-1);
    }
    buffer[f_size] = (unsigned char) '\0';
    return buffer;
}


int file_write(const char* dir, unsigned int* data_buffer, size_t buffer_size) {
    FILE* fp = fopen(dir, "wb");
    if (fp == NULL) {
        perror("fopen: ");
        printf("Could not create file %s", dir);
        exit(-1);
    }
    size_t written = 0;
    for (unsigned int i  = 0; i < buffer_size; i++) {
        written += fprintf(fp, "%u, ", data_buffer[i]);
    }
    if (written < buffer_size) {
        perror("fwrite: ");
        printf("Could only write %u chars", written);
        exit(-1);
    }
    file_close(fp);
    return 0;
}


int file_close(FILE* fp) {
    if (fclose(fp) != 0) {
        perror("fclose: ");
        exit(-1);
    }    
    return 0;
}
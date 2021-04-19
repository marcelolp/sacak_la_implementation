#include <stdio.h>
#include "file_io.h"

/* For now the only use of this function is debugging */
int main() {
    FILE* fp_test = file_open("tmp.txt", "r");
    char* file_test = file_read(fp_test);

    // printf("%u\n", file_size(fp_test));
    // printf("%s\n", file_test);

    return 0;

}
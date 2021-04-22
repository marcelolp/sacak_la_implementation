#include <stdio.h>

#include "file_io.h"
#include "time_test.h"
#include "sacak_la.h"

/* For now the only use of this function is debugging */
int main() {

    reset_time();
    start_time();

    FILE* fp_test = file_open("tmp.txt", "r");
    char* file_test = file_read(fp_test);

    printf("%u\n", file_size(fp_test));
    printf("%s\n", file_test);
    // Do something for >1-2ms to test timer
    unsigned int j = 0;
    for (int i = 0; i < 100000000; i++) {
        j += i;
    }
    // Output so j doesnt get optimized away
    printf("%u\n", j);

    stop_time();
    long int runtime = get_time();

    printf("%li ms\n", runtime);

    return 0;

}
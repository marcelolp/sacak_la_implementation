#include <stdio.h>

#include "file_io.h"
#include "time_test.h"
#include "sacak_la.h"
#include "symbols.h"

/* For now the only use of this function is debugging */
int main() {

    reset_time();
    start_time();

    // Do something for >1-2ms to test timer
    
    FILE* file_alphabet = file_open("alphabet.txt", "r");
    unsigned char* alphabet = file_read(file_alphabet);
    size_t alphabet_size = file_size(file_alphabet);

    printf("%s\n", alphabet);
    printf("%u\n", alphabet_size);

    set_alphabet(alphabet, alphabet_size);
    unsigned char* proc_alphabet = get_alphabet();

    for (int i = 0; i < 255; i++) {
        int index = proc_alphabet[i];
        if (index < 0) {
            continue;
        }
        printf("%c|", i);
        printf("%hhi ", index);

    }
    printf("\n");

    free_alphabet();
    file_close(file_alphabet);

    stop_time();
    long int runtime = get_time();

    printf("%li ms\n", runtime);

    return 0;

}
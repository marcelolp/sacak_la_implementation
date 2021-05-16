#include <stdio.h>

#include "file_io.h"
#include "sacak_la.h"
#include "symbols.h"
#include "time_test.h"

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
        if (index == 255) {
            continue;
        } else {
            printf("%c|", i);
            printf("%i ", index);
        }
    }
    printf("\n\n");

    FILE* file_text = file_open("tmp.txt", "r");
    unsigned char* text = file_read_d_term(file_text);
    size_t text_size = file_size(file_text) + 1; // $-symbol

    unsigned int* la = sacak_la_two_aux(text, proc_alphabet, text_size, get_alphabet_size());
    free(la);

    free_alphabet();
    file_close(file_alphabet);
    file_close(file_text);

    stop_time();
    long int runtime = get_time();

    printf("%li ms\n", runtime);

    return 0;

}
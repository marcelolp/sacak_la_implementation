#include <stdio.h>

#include "file_io.h"
#include "sacak_la.h"
#include "symbols.h"
#include "test.h"

extern int debug;

/* For now the only use of this function is debugging */
int main(int argc, char** argv) {
    debug = 0;      // TODO: read from cmd-args


    FILE* file_alphabet = file_open("src/alphabet.txt", "r");
    unsigned char* alphabet = file_read(file_alphabet);
    size_t alphabet_size = file_size(file_alphabet);

    if (debug) {
        printf("Alphabet in order: %s\n", alphabet);
        printf("Alphabet size: %u\n", alphabet_size);
    }

    set_alphabet(alphabet, alphabet_size);
    unsigned char* proc_alphabet = get_alphabet();

    if (debug) {
        printf("Assigned values: ");
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
    }

    FILE* file_text = file_open("src/tmp.txt", "r");
    unsigned int* text = file_read_d_term(file_text);
    size_t text_size = file_size(file_text) + 1; // $-symbol

    if (debug) {
        printf("\nT:  ");
        for (int i = 0; i < text_size; i++) {
            printf("%2i ", i);
        }
        printf("\n    ");
        for (int i = 0; i < text_size; i++) {
            printf("%2c ", (char) text[i]);
        }
        printf("\n");
    }
    unsigned int* la = NULL;
    if (debug) {
        la = sacak_la_two_aux(text, proc_alphabet, text_size, get_alphabet_size());
        printf("\n\n");
    }
    
    // still get the correct time, just run it again with correct output
    debug = 0;
    la = sacak_la_two_aux(text, proc_alphabet, text_size, get_alphabet_size());
    double runtime = get_timer();
    printf("Running time: %.4lf s\n", runtime);
    printf("Running time per input byte: %.15lf ms or %.15lf \xE6s\n", (double) ((runtime / text_size) * 1000), 
        (double) ((runtime / text_size) * (1000 * 1000)));


    /*
    if (test_lyndon_array(text, proc_alphabet, la, text_size, get_alphabet_size())) {
        printf("Lyndon-Array is correct.");
    } else {
        printf("Lyndon-Array is incorrect.");
    }
    */
    free(la);

    free_alphabet();
    file_close(file_alphabet);
    file_close(file_text);



    return 0;

}
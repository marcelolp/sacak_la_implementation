#include <stdio.h>

#include "file_io.h"
#include "sacak_la.h"
#include "symbols.h"
#include "test.h"

extern int debug;
extern int output;
extern int test;

int main(int argc, char** argv) {

    //debug = 0;
    //output = 0;
    //test = 0;
    int gen_input = 0;
    int gen_input_len = 0;
    int mode = -1;
    const char* input_file = NULL;
    const char* alphabet_file = NULL;

    if (argc == 1) {
        printf("Since no input file is specified, a default example will be used. For further information, use -h\n");
        output = 1;
    }
    for (int i = 1; i < argc; i++) {
        if (argv[i][0] != '-') {
            printf("Command line arguments have incorrect format, use -h for help\n");
            exit(0);
        }
        char opt = argv[i][1];
        switch(opt) {
            case 'h' :                                                                              // h -> help
                printf("\nFollowing options are implemented:\n");
                printf("-h      : show this text\n");
                printf("-i <arg>: specify the path <arg> to the input file\n");
                printf("-a <arg>: specify the path <arg> to the alphabet file\n");
                printf("-d      : show debug output, not recommended for inputs larger than 200 byte\n");
                printf("-o      : show the resulting array on the terminal\n");
                printf("-g <arg>: generate input with size <arg> (integer)\n");
                printf("-m <arg>: use only one algorithm (1 = inplace, 2 = single aux. array, 3 = two aux. array, 4 = sacak only),\n");
                printf("          default is all three algorithms\n");
                printf("-t      : test result (sa, la) for errors\n");
                printf("\nCombined options (eg. -doh) are not supported\n");
                exit(0);
            case 'i' :                                                                              // i -> input file directory
                if (i + 1 >= argc) {
                    printf("Option -i needs a file path as parameter\n");
                    exit(0);
                }
                input_file = argv[i+1];
                i++;
                break;
            case 'a' :                                                                              // a -> alphabet file directory
                if (i + 1 >= argc) {
                    printf("Option -a needs a file path as parameter\n");
                    exit(0);
                }
                alphabet_file = argv[i+1];
                i++;
                break;
            case 'd' :                                                                              // d -> use debug mode
                debug = 1;
                break;
            case 'o' :                                                                              // o -> output result to terminal
                output = 1;
                break;
            case 'g' :                                                                              // g -> generate input
                if (i + 1 || (gen_input_len = atoi(argv[i+1]) == 0)) {
                    printf("Option -g needs an integer as parameter\n");
                    exit(0);
                }
                gen_input = 1; 
                i++;
                break;
            case 'm' :                                                                              // m -> select algorithm
                if (i + 1 >= argc) {
                    printf("Option -m needs a integer (1, 2, 3) as parameter\n");
                    exit(0);
                }
                mode = atoi(argv[i+1]);
                if (mode < -1 || mode == 0 || mode > 4) {
                    printf("Argument for option m has invalid value\n");
                    exit(0);
                }
                i++;
                break;
            case 't' :                                                                              // t -> test result
                test = 1;
                break;

        }
    }

    printf("\n---------------------------------------------------------------------------------\n");

    size_t alphabet_size = 255;
    if (alphabet_file != NULL) {
        FILE* file_alphabet = file_open(alphabet_file == NULL ? "../src/alphabet.txt" : alphabet_file, "r");
        unsigned char* alphabet = file_read(file_alphabet);
        alphabet_size = file_size(file_alphabet);
        file_close(file_alphabet);
        set_alphabet(alphabet, alphabet_size);
        free(alphabet);
    } else {
        set_alphabet(NULL, 255);
    }


    if (debug) {
        printf("Alphabet in order: %s\n", alphabet);
        printf("Alphabet size: %u\n", alphabet_size);
    }


    unsigned char* proc_alphabet = get_alphabet();

    if (debug) {
        printf("Assigned values: ");
        for (int i = 0; i < 255; i++) {
            int index = proc_alphabet[i];
            if (index != 255) {
                printf("%c|", i);
                printf("%i ", index);
            }
        }
        printf("\n\n");
    }

    unsigned int* text = NULL;
    size_t text_size = 50;

    if (gen_input) {
        text = gen_test_string(proc_alphabet, gen_input_len, alphabet_size);
    } else {
        FILE* file_text = file_open(input_file == NULL ? "../src/tmp.txt" : input_file, "r");
        text = file_read_d_term(file_text);
        text_size = file_size(file_text) + 1; // $-symbol
        file_close(file_text);
    }

    if (output || debug) {
        printf("\nT:  ");
        for (int i = 0; i < text_size; i++) {
            printf("%2i ", i);
        }
        printf("\n    ");
        for (int i = 0; i < text_size; i++) {
            printf("%2c ", (char) text[i]);
        }
        printf("\n\n");
    } 

    unsigned int* la = NULL; 
    double runtime = 0;
    int debug_restore = debug;
    
    if (mode  == 4) {
        if (debug) {
            sacak(text, proc_alphabet, text_size, get_alphabet_size());
            printf("\n\n");
        }

        debug = 0;
        printf("\nSacak only:\n");
        sacak(text, proc_alphabet, text_size, get_alphabet_size());
        runtime = get_timer();
        printf("Running time: %.4lf s\n", runtime);
        if (runtime == .0f) {
            printf("Running time too small to get accurate values for running time per input byte\n");
        } else {
            printf("Running time per input byte: %.15lf ms or %.15lf \xE6s\n", (double) ((runtime / text_size) * 1000), 
                (double) ((runtime / text_size) * (1000 * 1000)));
        }
        debug = debug_restore;
    }

    if (mode == -1 || mode  == 1) {
        if (debug) {
            la = sacak_la_inplace(text, proc_alphabet, text_size, get_alphabet_size());
            free(la);
            la = NULL;
            printf("\n\n");
        }

        debug = 0;
        printf("\nUsing no auxilliary array:\n");
        la = sacak_la_inplace(text, proc_alphabet, text_size, get_alphabet_size());
        free(la);
        runtime = get_timer();
        printf("Running time: %.4lf s\n", runtime);
        if (runtime == .0f) {
            printf("Running time too small to get accurate values for running time per input byte\n");
        } else {
            printf("Running time per input byte: %.15lf ms or %.15lf \xE6s\n", (double) ((runtime / text_size) * 1000), 
                (double) ((runtime / text_size) * (1000 * 1000)));
        }
        debug = debug_restore;
    }

    if (mode == -1 || mode  == 2) {
        if (debug) {
            la = sacak_la_one_aux(text, proc_alphabet, text_size, get_alphabet_size());
            free(la);
            la = NULL;
            printf("\n\n");
        }

        debug = 0;
        printf("\nUsing one auxilliary array:\n");
        la = sacak_la_one_aux(text, proc_alphabet, text_size, get_alphabet_size());
        free(la);
        runtime = get_timer();
        printf("Running time: %.4lf s\n", runtime);
        if (runtime == .0f) {
            printf("Running time too small to get accurate values for running time per input byte\n");
        } else {
            printf("Running time per input byte: %.15lf ms or %.15lf \xE6s\n", (double) ((runtime / text_size) * 1000), 
                (double) ((runtime / text_size) * (1000 * 1000)));
        }
        debug = debug_restore;
    }

    if (mode == -1 || mode  == 3) {
        if (debug) {
            la = sacak_la_two_aux(text, proc_alphabet, text_size, get_alphabet_size());
            free(la);
            la = NULL;
            printf("\n\n");
        }
        
        debug = 0;
        printf("\nUsing two auxilliary arrays:\n");
        la = sacak_la_two_aux(text, proc_alphabet, text_size, get_alphabet_size());
        free(la);
        runtime = get_timer();
        printf("Running time: %.4lf s\n", runtime);
        if (runtime == .0f) {
            printf("Running time too small to get accurate values for running time per input byte\n");
        } else {
            printf("Running time per input byte: %.15lf ms or %.15lf \xE6s\n", (double) ((runtime / text_size) * 1000), 
                (double) ((runtime / text_size) * (1000 * 1000)));
        }
        debug = debug_restore;
    }

    free(text);
    free_alphabet();
    return 0;
}
#include "test.h"

unsigned int* gen_test_string(unsigned char* alphabet, size_t n, size_t a) {
    unsigned int* text = (unsigned int*) malloc(n * sizeof(unsigned int));                       // bkt has size sigma + 1, sigma < 255
    unsigned int* a_pointers = (unsigned int*) malloc((a-1) * sizeof(unsigned int));
    if (text == 0 || a_pointers == 0) {
        perror("malloc: ");
        printf("Could not allocate memory for input\n");
        exit(-1);
    }

    // get pointers to the used entries i alphabet
    int j = 0;
    for (int i = 0; i < 255; i++) {
        if (i != '$' && alphabet[i] != 255) {
            a_pointers[j] = i;
            j++;
        }
    }

    // generate a random character for each position
    for (int i = 0; i < n; i++) {
        int r = rand() % a;
        text[i] = a_pointers[r];
    }
    text[n-1] = '$';
    return text;
}


int test_suffix_array(unsigned int* t, unsigned int* sa, unsigned char* alphabet, size_t n, size_t a) {
    
    // initial fast test to look if all beginning characters are in lexicographical order
    for (int i = 1; i < n; i++) {
        if (alphabet[ (int) t[sa[i-1]] ] > alphabet[ (int) t[sa[i]] ]) {
            return 0;
        }
    }

    // compare the suffixes in sa one by one
    // can be very slow for large n since O(n^2) running time.
    for (int i = 1; i < n; i++) {
        for (int j = 0; sa[i-1] + j == n || sa[i] + j == n; j++) {
            if (alphabet[ t[sa[i-1] + j] ] > alphabet[ t[sa[i] + j] ]) {
                return 0;
            }
        }
    }
    return 1;
}


int test_lyndon_array(unsigned int* t, unsigned int* la, unsigned char* alphabet, size_t n, size_t a) {

    return 0;
}
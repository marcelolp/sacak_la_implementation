#include "test.h"

/*
*  Tests if an suffix array is correct, returns 0 if not, 1 otherwise
*/
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

/*
*  Tests if an lyndon array is correct, returns 0 if not, 1 otherwise
*/
int test_lyndon_array(unsigned int* t, unsigned int* la, unsigned char* alphabet, size_t n, size_t a) {

    return 0;
}
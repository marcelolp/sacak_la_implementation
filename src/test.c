#include "test.h"

unsigned int* gen_test_string(unsigned char* alphabet, size_t n, size_t a) {
    unsigned int* text = (unsigned int*) malloc(n * sizeof(unsigned int));                          
    unsigned int* a_pointers = (unsigned int*) malloc((a-1) * sizeof(unsigned int));
    if (text == 0 || a_pointers == 0) {
        perror("malloc: ");
        printf("Could not allocate memory for input\n");
        exit(-1);
    }

    // get pointers to the used entries in alphabet
    int j = 0;
    for (int i = 0; i < 255; i++) {
        if (i != '$' && alphabet[i] != 255) {
            a_pointers[j] = i;
            j++;
        }
    }

    // generate a random character for each position
    srand(time(NULL));
    for (int i = 0; i < n; i++) {
        int r = rand() % a;
        text[i] = a_pointers[r];
    }
    text[n-1] = '$';
    return text;
}


int test_suffix_array(unsigned int* t, unsigned int* sa, unsigned char* alphabet, size_t n, size_t a) {

    // compare the suffixes in sa one by one (can take O(n^2) time)
    for (unsigned int i = 1; i < n; i++) {
        if (n > 100000 && i % (unsigned int) (n/log(n)*2) == 0) {                                   // if n is big show that the program is making some progress
            printf("%.2g %% of the suffix array tested\n", ((float) i/ (float)n)*(float)100);
        }
        for (unsigned int j = sa[i], k = sa[i-1]; j < n && k < n; j++, k++) {
            if (alphabet[t[j]] > alphabet[t[k]]) {                                                  // confirms suf(t[sa[i]]) > suf(t[sa[i-1]])
                break;
            }
            if (alphabet[t[j]] < alphabet[t[k]]) {                                                  // if this occurs, the suffix array is wrong
                printf("%u: alphabet[t[%u..%u]] = %u < alphabet[t[%u..%u]] = %u\n", i, sa[i], j, alphabet[t[j]], sa[i-1], k, alphabet[t[k]]);
                return 0;
            }
        }
    }
    return 1;
}


int test_lyndon_array(unsigned int* t, unsigned int* la, unsigned char* alphabet, size_t n, size_t a) {
    
    // go along la[+l+1..n] and check if there is a rotation that is smaller than la[i..i+l] in that range
    for (unsigned int i = 0; i < n; i++) {
        unsigned int l = la[i];
        if (i == n-1 && l != 1) {
            return 0;
        }
        // Test if the rotation T[j..n]T[i..j-1] < T[i..i+l-1] for j = i+l..n
        // Equivalent to suf(T[i]) > suf(T[i+l]) -> test that instead
        for (unsigned int j = i; j <= i+l && j+l < n; j++) {
            if (alphabet[t[j]] > alphabet[t[l+j]]) {                                                // confirms suf(t[i]) > suf(t[i + la[i]])
                break;
            }
            if (alphabet[t[j]] < alphabet[t[l+j]]) {                                                // if this occurs, the lyndon array is wrong
                return 0;
            }
        }
    }
    return 1;
}
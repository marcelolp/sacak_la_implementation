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
    
    // initial fast test to look if all beginning characters are in lexicographical order
    for (int i = 1; i < n; i++) {
        if (alphabet[ (int) t[sa[i-1]] ] > alphabet[ (int) t[sa[i]] ]) {
            return 0;
        }
    }

    // compare the suffixes in sa one by one
    // should be close to O(n) since the average length of an LMS-substring usually is rather short
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
    
    // go along la and check for every la[i] = l if there is a character in t[i..i+l] that is 
    // smaller or equal to t[i]
    for (int i = 0; i < n; i++) {
        int l = la[i];
        int s_block = 1;                                                                            // it is fine if the symbols directly right of t[i] are equal to t[i]         
        int s_block_length = 0;                
        for (int j = 1; j < l; j++) {
            if (i + j > n-1) {                                                                      // dont go out of bounds
                break;
            }
            if (t[i] > t[i + j]) {                                                                  // symbol in t[i+1..i+l] is smaller than t[i] -> incorrect
                return 0;
            }
            if (s_block && t[i] == t[i+j]) {                                                        // check if i+j is still in a block of equal character right of t[i]
                s_block_length++;
                continue;
            } else {                                                                                // not anymore
                s_block = 0;
            }
            if (t[i] == t[i+j]) {                                                                   // t[i] == t[i+j] but there are characters with higher value inbetween
                if (s_block_length > 0) {                                                           // no issue, there was a longer block of those characters further left
                    s_block_length--;
                } else {                                                                            // there would be a rotation with smaller value
                    return 0;
                }
            }
        }
    }
    return 1;
}
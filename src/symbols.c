#include "symbols.h"

alphabet_size = 0;                                                                                  // Not used atm
ascii = 1; 

void set_alphabet( unsigned char* a, size_t a_size) {
    // Use the default alphabet
    if (!a) {                                                                                       
        ascii = 1;
    }
    
    // Use alphabet a and insert termination symbol $ at lowest index
    alphabet = (unsigned char*) malloc((256) * sizeof(int));                                                 // alphabet array has size 256
    if (!alphabet) {
        perror("malloc: ");
        printf("Could not allocate memory for alphabet array");
        exit(-1);
    }
    
    if (a_size > 255 - 1) {                                                                         // Reject the alphabet if it exceeds a size that can be stored in a (unsigned) byte
        printf("Alphabet too big (should be at most 254) ");
        exit(-1);
    }
    alphabet_size = a_size + 1;

    for (int i = 0; i < 255; i++) {                                                                 // initialize the array with -1
        alphabet[i] = (unsigned char) 255;
    }
    alphabet['$'] = (unsigned char) 0;                                                                       // '$' has the lowest index
    for (unsigned char i = 0; i < a_size; i++) {                                                             // overwrite the positions for which a symbol in the alphabet exists
        if (a[i] > 255 || a[i] < 0) {
            printf("Symbol out of range 0-255 (should be 48-57, 65-90, 97-122)");
            exit(-1);
        }
        alphabet[(int) a[i]] = (unsigned char) i+1;                                                          // alphabet['c'] := index(c), lower index means lower lex. value
    }   
    ascii = 0;                                                                                       
}

void free_alphabet() {
    free(alphabet);
}

unsigned char* get_alphabet(){
    if(a_size = 0) {
        return NULL;
    }
    return alphabet;
}

size_t get_alphabet_size(){
    return alphabet_size;
}

int lex_compare_strings( unsigned char* a,  unsigned char* b) {
    int state = 0;
    int i = 0;
    while(a[i] != '\0' && b[i] != '\0') {
        state = lex_compare_symbols(a[i], b[i]);
        if (state == -1) {                                                                          // a[i] > b[i] -> a > b
            return -1;
        } else if (state == 1) {                                                                    // a[i] < b[i] -> a < b
            return 1;
        } 
        i++;                                                                                        // a[i] = b[i]
    }
    if (a[i] == '\0') {                                                                             // a, b identical until a terminates -> a < b
        if (b[i] == '\0') {                                                                         // a, b identical until termination symbol -> a = b
            return 0;
        }
        return -1;
    } else if (b[i] == '\0') {                                                                      // a, b identical until b termiantes -> a > b
        return 1;
    }
    return -2;                                                                                      // should not happen but just in case
}

int lex_compare_symbols(unsigned char a, unsigned char b) {
    if (ascii) {                                                                                    // works for '$' too as it has lower value than every other (reasonable) symbol
        if (a > b) {
            return 1;
        } else if (a == b) {
            return 0;
        } else {
            return -1;
        }
    } else {
        if (alphabet[a] > alphabet[b]) {
            return 1;
        } else if (alphabet[a] == alphabet[b]) {
            return 0;
        } else {
            return -1;
        }
    }
}
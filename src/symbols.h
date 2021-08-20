#ifndef SYMBOLS_H
#define SYMBOLS_H

#include <stdio.h>
#include <stdlib.h>

/*
*  Contains functions to lexicographically compare symbols based on an alphabet
*/

unsigned char * alphabet;                                                                           // alphabet[c] = i is the lex. value i of the symbol 'c'

/*
*  Sets the alphabet as the string of symbols given by the pointer (symbols are expected to be 1 byte long)
*  If the pointer is null, uses the order f symbols in the ascii encoding
*  Returns -1 if the string is not usable as alphabet (duplicate symbols etc.)
*/
void set_alphabet( unsigned char* a, size_t a_size);

/*
*  Deallocates the memory for the currently set alphabet to set a new one
*/
void free_alphabet();

/*
*  Returns the currently set alphabet.
*  NULL if no alphabet is set
*/
unsigned char* get_alphabet();

/*
*  Returns the size of the alphabet sigma + 1
*/
size_t get_alphabet_size();

/*
*  the strings are expected to be terminated by /0 and to be called with &t1[i], &t2[j] to compare 
*  from i, j onward
*  Returns 1 if the first string is lexicographically larger, 0 if they are equal and -1 otherwise
*  Unused
*/
int lex_compare_strings( unsigned char* a,  unsigned char* b);

/*
*  Returns 1 if the first symbol a is lexicographically larger, 0 if they are equal and -1 otherwise
*/
int lex_compare_symbols(unsigned int a, unsigned int b);


#endif
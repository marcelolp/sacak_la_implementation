#ifndef TEST_H
#define TEST_H

#include <stdio.h>
#include <time.h>
#include <math.h>

#include "symbols.h"

/* 
*  Generate a string of length n with random characters from the alphabet + $ as termination symbol 
*/
unsigned int* gen_test_string(unsigned char* alphabet, size_t n, size_t a);

/* 
*  Tests the given suffix array for correctness
*  Returns 1 if sa is correct, 0 otherwise 
*/
int test_suffix_array(unsigned int* t, unsigned int* sa, unsigned char* alphabet, size_t n, size_t a);

/* 
*  Tests the given Lyndon array for correctness
*  Returns 1 if la is correct, 0 otherwise 
*/
int test_lyndon_array(unsigned int* t, unsigned int* la, unsigned char* alphabet, size_t n, size_t a);

#endif
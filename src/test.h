#ifndef TEST_H
#define TEST_H

#include <stdio.h>

/* Returns 1 if sa is correct, 0 otherwise */
int test_suffix_array(unsigned int* t, unsigned int* sa, unsigned char* alphabet, size_t n, size_t a);

/* Returns 1 if la is correct, 0 otherwise */
int test_lyndon_array(unsigned int* t, unsigned int* la, unsigned char* alphabet, size_t n, size_t a);

#endif
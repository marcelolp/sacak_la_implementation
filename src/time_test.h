#ifndef TIME_TEST_H
#define TIME_TEST_H

#include <stdio.h>
#include <stdlib.h>

#include "file_io.h"
#include "sacak_la.h"

/*
*  Contains functions to compare the running time of different sacak-la implementations 
*/

/*
*  Returns a array of milliseconds where very entry is the time one algorithm needed to compute 
*  the Lyndon Array of t. 
*  1: sacak_la_two_aux
*  2: sacak_la_one_aux
*  3: sacak_la_inplace 
*/
float* compare_times(unsigned int* t, unsigned char * alphabet, size_t n, size_t a);

float run_two_aux(unsigned int* t, unsigned char * alphabet, size_t n, size_t a);

float run_one_aux(unsigned int* t, unsigned char * alphabet, size_t n, size_t a);

float run_inplace(unsigned int* t, unsigned char * alphabet, size_t n, size_t a);


#endif
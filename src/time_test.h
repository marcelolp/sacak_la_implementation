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
float* compare_times(const char* t, size_t n);

float run_two_aux(const char* t, size_t n);

float run_one_aux(const char* t, size_t n);

float run_inplace(const char* t, size_t n);


#endif
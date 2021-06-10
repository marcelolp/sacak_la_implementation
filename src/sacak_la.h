#ifndef SACAK_LA_H
#define SACAK_LA_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "symbols.h"
#include "test.h"

double sum_time;
clock_t start_time;
size_t max_memory;
size_t max_bkt_memory;
size_t max_alphabet_memory;

extern int ascii;

/*
*  Returns the time variable 
*/
double get_timer();

/* 
*  Starts the timer
*/
int start_timer();

/*
*  Stops the timer and adds the time passed since the last call of time_start() to the timer variable
*/
int stop_timer();

/*
*  Resets the time variable to 0
*/
int reset_timer();

/*
*  Computes the Lyndon Array of t using two auxiliary arrays
*  n is the size of t (includin the termination symbol)
*  a is the size of the alphabet
*  Returns a pointer to LA
*/
unsigned int* sacak_la_two_aux(unsigned int* t, unsigned char* alphabet, size_t n, size_t a);

/*
*  Computes the Lyndon Array of t using one auxiliary array
*  n is the size of t (includin the termination symbol)
*  a is the size of the alphabet
*  Returns a pointer to LA
*/
unsigned int* sacak_la_one_aux(unsigned int* t, unsigned char* alphabet, size_t n, size_t a);

/*
*  Computes the Lyndon Array of t without auxiliary arrays
*  n is the size of t (includin the termination symbol)
*  a is the size of the alphabet
*  Returns a pointer to LA
*/
unsigned int* sacak_la_inplace(unsigned int* t, unsigned char* alphabet, size_t n, size_t a);


#endif
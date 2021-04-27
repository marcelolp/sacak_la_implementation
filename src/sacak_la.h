#ifndef SACAK_LA_H
#define SACAK_LA_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "symbols.h"

long int sum_time;
long int last_time;

/*
*  Returns the time variable 
*/
long int get_time();

/* 
*  Starts the timer
*/
int start_time();

/*
*  Stops the timer and adds the time passed since the last call of time_start() to the timer variable
*/
int stop_time();

/*
*  Resets the time variable to 0
*/
int reset_time();

/*
*  Computes the Lyndon Array of t using two auxiliary arrays
*  n is the size of t (includin the termination symbol)
*  a is the size of the alphabet
*  Returns a pointer to LA
*/
unsigned char* sacak_la_two_aux(unsigned char* t, unsigned char* alphabet, size_t n, size_t a);

/*
*  Computes the Lyndon Array of t using one auxiliary array
*  n is the size of t (includin the termination symbol)
*  a is the size of the alphabet
*  Returns a pointer to LA
*/
unsigned char* sacak_la_one_aux(unsigned char* t, unsigned char* alphabet, size_t n, size_t a);

/*
*  Computes the Lyndon Array of t without auxiliary arrays
*  n is the size of t (includin the termination symbol)
*  a is the size of the alphabet
*  Returns a pointer to LA
*/
unsigned char* sacak_la_inplace(unsigned char* t, unsigned char* alphabet, size_t n, size_t a);


#endif
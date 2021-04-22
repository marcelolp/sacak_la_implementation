#ifndef SACAK_LA_H
#define SACAK_LA_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

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
*  Returns a pointer to LA
*/
char* sacak_la_two_aux(const char* t, const char* sa, size_t n);

/*
*  Computes the Lyndon Array of t using one auxiliar array
*  Returns a pointer to LA
*/
char* sacak_la_one_aux(const char* t, const char* sa, size_t n);

/*
*  Computes the Lyndon Array of t without auxiliary arrays
*  Returns a pointer to LA
*/
char* sacak_la_inplace(const char* t, const char* sa, size_t n);

/*
*  Does the part of the computation of SA thats common between all sacak-la variants
*/
int sacak_common(const char* t, const char* sa, const char* bkt, size_t n);


#endif
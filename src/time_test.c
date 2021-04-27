#include "time_test.h"

float* compare_times(unsigned char* t, unsigned char * alphabet, size_t n, size_t a) {
    float* times = (float*) malloc(3 * sizeof(float));
    times[0] = run_two_aux(t, alphabet, n, a);
    times[1] = run_one_aux(t, alphabet, n, a);
    times[2] = run_inplace(t, alphabet, n, a);
    return times;
}

float run_two_aux(unsigned char* t, unsigned char * alphabet, size_t n, size_t a) {
    reset_time();
    start_time();
    sacak_la_two_aux(t, alphabet, n, a);
    stop_time();
    return get_time();
}

float run_one_aux(unsigned char* t, unsigned char * alphabet, size_t n, size_t a) {
    reset_time();
    start_time();
    sacak_la_one_aux(t, alphabet, n, a);
    stop_time();
    return get_time();
}

float run_inplace(unsigned char* t, unsigned char * alphabet, size_t n, size_t a) {
    reset_time();
    start_time();
    sacak_la_inplace(t, alphabet, n, a);
    stop_time();
    return get_time();
}


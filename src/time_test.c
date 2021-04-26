#include "time_test.h"

float* compare_times(const char* t, size_t n) {
    float* times = (float*) malloc(3 * sizeof(float));
    times[0] = run_two_aux(t, n);
    times[1] = run_one_aux(t, n);
    times[2] = run_inplace(t, n);
    return times;
}

float run_two_aux(const char* t, size_t n) {
    return 0;
}

float run_one_aux(const char* t, size_t n) {
    return 0;
}

float run_inplace(const char* t, size_t n) {
    return 0;
}


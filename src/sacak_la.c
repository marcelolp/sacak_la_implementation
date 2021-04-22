#include "sacak_la.h"

long int get_time() {
    return sum_time;
}

int start_time() {
    last_time = clock();
    return 0;
}

int stop_time() {
    if (last_time = 0) {
        printf("stop_time() call with no start_time() call before");
        return -1;
    }
    long int delta_time = (clock() - last_time);
    sum_time += delta_time;
    last_time = 0;
    return 0;
    
}

int reset_time() {
    sum_time = 0;
    last_time = 0;
    return 0;
}

char* sacak_la_two_aux(const char* t, const char* sa, size_t n);
char* sacak_la_one_aux(const char* t, const char* sa, size_t n);
char* sacak_la_inplace(const char* t, const char* sa, size_t n);

int sacak_common(const char* t, const char* sa, const char* bkt, size_t n);
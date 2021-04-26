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

char* sacak_la_two_aux(const char* t, size_t n) {
        // Memory allocation
        char* sa = (char*) malloc(n * sizeof(char));
        char* la = (char*) malloc(n * sizeof(char));
        char* next = (char*) malloc(n * sizeof(char));
        char* prev = (char*) malloc(n * sizeof(char));
        
        if (sa == 0 || la == 0 || next == 0 || prev == 0) {
            perror("malloc: ");
            printf("Could not allocate memory for la, sa, next, prev");
            exit(-1);
        }
        // TODO: exchange every symbol in t with its index in the alphabet  

        // Bucket array



        // Free memory (assuming sa is not of interest)
        free(sa);
        free(next);
        free(prev);

    	return la;
}


char* sacak_la_one_aux(const char* t, size_t n);
char* sacak_la_inplace(const char* t, size_t n);

int sacak_common(const char* t, const char* sa, const char* bkt, size_t n);
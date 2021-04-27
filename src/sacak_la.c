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

unsigned char* sacak_la_two_aux(unsigned char* t, unsigned char* alphabet, size_t n, size_t a) {
        
        // Memory allocation
        unsigned char* sa = (unsigned char*) malloc(n * sizeof(unsigned char));
        unsigned char* la = (unsigned char*) malloc(n * sizeof(unsigned char));
        unsigned char* next = (unsigned char*) malloc(n * sizeof(unsigned char));
        unsigned char* prev = (unsigned char*) malloc(n * sizeof(unsigned char));

        int* bkt = (int*) malloc(a * sizeof(int));
        
        if (sa == 0 || la == 0 || next == 0 || prev == 0 || bkt == 0) {
            perror("malloc: ");
            printf("Could not allocate memory for la, sa, next, prev");
            exit(-1);
        }

        // Recursive sorting of the LMS-type suffixes, algorithm published by Nong et al. in 
        // "Two efficient algorithms for linear time suffixarray construction" (access with IEEE)

        sacak_rec(t, alphabet, sa, floor(n/2), a, 0);


        // The last level has to be done manually as LA is computed during it

        // Free memory (assuming sa is not of interest)
        free(sa);
        free(next);
        free(prev);

    	return la;
}


unsigned char* sacak_la_one_aux(unsigned char* t, unsigned char* alphabet, size_t n, size_t a);     
unsigned char* sacak_la_inplace(unsigned char* t, unsigned char* alphabet, size_t n, size_t a);     

int sacak_common(unsigned char* t, unsigned char* sa, unsigned char* bkt, size_t n);

/*
*  Computes the bucket array for an string s
*  If head == 1, stores pointers to the head of the buckets, otherwise to the tails
*/ 
int get_bkt_array(unsigned char* t, int* bkt, unsigned char* alphabet, size_t n, size_t a, int head) {
    for (int i = 0; i < a; i++) {
        bkt[i] = 0;
    }
    for (int i = 0; i < n; i++) {
        bkt[alphabet[(int) t[i]]] += 1;                                                             // alphabet[(int) t[i]] is the value of the symbol at i,
    }    
    int offset = 0;
    for (int i = 0; i < a; i++) {
        offset += bkt[i];
        if (head) {
            bkt[i] = offset - bkt[i];                                                               // pointer to the head of the bucket is the last offset
        } else {
            bkt[i] = offset - 1;                                                                    // pointer to the tail of the bucket is the offset - 1
        }                                                                             
    }                                                                                          

}

/*
*  Compute the type of the symbol at pos
*  Returns 0 for L-type, 1 for S-type
*/
int get_type(unsigned char* t, size_t n, int pos) {
    if (pos < n-1) {
        if (lex_compare_symbols(t[pos], t[pos+1]) > 0) {
            return 0;
        } else {
            return 1;
        }
    }
    return 1;                                                                                       // $ is always S-type
}

/*
*  Recursive part of the computation of SA
*  Initial call with depth = 0 
*/
int sacak_rec(unsigned char* t1, unsigned char* alphabet, unsigned char* sa1, size_t n1, size_t a, int depth) {

}
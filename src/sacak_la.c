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

unsigned int* sacak_la_two_aux(unsigned char* t, unsigned char* alphabet, size_t n, size_t a) {
        
        // Memory allocation
        unsigned int* sa = (unsigned int*) malloc(n * sizeof(unsigned int));
        unsigned int* la = (unsigned int*) malloc(n * sizeof(unsigned int));
        unsigned int* next = (unsigned int*) malloc(n * sizeof(unsigned int));
        unsigned int* prev = (unsigned int*) malloc(n * sizeof(unsigned int));

        unsigned int* bkt = (unsigned int*) malloc(a * sizeof(unsigned int));                    // bkt has size sigma + 1, sigma < 255
        
        if (sa == 0 || la == 0 || next == 0 || prev == 0 || bkt == 0) {
            perror("malloc: ");
            printf("Could not allocate memory for la, sa, next, prev");
            exit(-1);
        }

        printf("Memory allocation alright");
        printf("\nT:  ");
        for (int i = 0; i < n; i++) {
            printf("%i ", i);
        }
        printf("\n    ");
        for (int i = 0; i < n; i++) {
            printf("%c ", t[i]);
        }
        printf("\n");


        // Recursive sorting of the LMS-type suffixes, algorithm published by Nong et al. in 
        // "Two efficient algorithms for linear time suffixarray construction" (access with IEEE)

        // sacak_rec(t, alphabet, sa, floor(n/2), a, 0);

        
        // step 1: induced sort lms-type suffixes
        for(int i = 0; i < n; i++) {                                                                // initialize sa with zeroes
            sa[i] = UINT_MAX;
        }
        get_bkt_array(t, bkt, alphabet, n, a, 0);                                                   // get pointers to the end of the buckets 
        int cur_type = 0;                                                                           
        int last_type = 0;
        for (int i = 0; i < n; i++) {                                                               // left to right scan of t                                                    
            cur_type = get_type(t, n, i);
            printf("%c is of type %c\n", t[i], cur_type ? 'S' : 'L');
            if (cur_type == 1 && last_type == 0) {                                                  // insert LMS-suffixes into their buckets in sa
                printf("%c LMS-type \n", t[i]);
                sa[bkt[alphabet[(int) t[i]]]] = i;                                                  // alphabet[(int) t[i]] is the value if the symbol at t[i]
                bkt[alphabet[(int) t[i]]]--;                                                        // shift bucket tail pointer one position to the left
            }
            last_type = cur_type;
        }
        print_suffix_array(sa, n, -1);

        // step 2: induce L-type suffixes
        get_bkt_array(t, bkt, alphabet, n, a, 1);
        for (int i = 0; i < n; i++) {                                                               // left-to-right scan of t
            if (sa[i] != UINT_MAX && sa[i] > 0 && !get_type(t, n, sa[i]-1)) {                       // sa[i] contains relevant entry and t[sa[i]-1] is L-type
                sa[bkt[alphabet[(int) t[sa[i]-1] ]]] = sa[i] - 1;                                   // insert the suffix t[sa[i]-1] into the head of its bucket
                bkt[alphabet[(int) t[sa[i]-1 ]]]++;                                                 // shift bucket head pointer one position to the right
                print_suffix_array(sa, n, i);
            }
        }

        // step 3: induce S-type suffixes
        get_bkt_array(t, bkt, alphabet, n, a, 0);
        for (int i = n-1; i > 0; i--) {                                                             // right-to-left scan of t
            if (sa[i] != UINT_MAX && sa[i] > 0 && get_type(t, n, sa[i]-1)) {                        // sa[i] contains relevant entry and t[sa[i]-1] is S-type
                sa[bkt[alphabet[(int) t[sa[i]-1] ]]] = sa[i] - 1;                                   // insert the suffix t[sa[i]-1] into the tail of its bucket
                bkt[alphabet[(int) t[sa[i]-1 ]]]--;                                                 // shift bucket tail pointer one position to the left
                print_suffix_array(sa, n, i);
            }
        }

        // TODO: computation of LA during step 3 with next-prev-arrays
        // TODO: create a function that does the common steps 1-2

        // print the resulting arrays for debugging
        printf("\nSA: ");
        for (int i = 0; i < n; i++) {
            printf("%i, ", sa[i]);
        }
        printf("\nLA: ");
        for (int i = 0; i < n; i++) {
            printf("%i, ", la[i]);
        }
        printf("\nbkt:");
        for (int i = 0; i < a; i++) {
            printf("%i, ", bkt[i]);
        }
        printf("\n\n");

        // Free memory (assuming sa is not of interest)
        free(sa);
        free(next);
        free(prev);
        free(bkt);
        sa = NULL;
        next = NULL;
        prev = NULL;
        bkt = NULL;
        la = NULL;

    	return la;
}


unsigned int* sacak_la_one_aux(unsigned char* t, unsigned char* alphabet, size_t n, size_t a) {
    return 0;
}     
unsigned int* sacak_la_inplace(unsigned char* t, unsigned char* alphabet, size_t n, size_t a) {
    return 0;
}     

int sacak_common(unsigned char* t, unsigned int* sa, unsigned char* bkt, size_t n);

void get_bkt_array(unsigned char* t, unsigned int* bkt, unsigned char* alphabet, size_t n, size_t a, int head) {
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

void print_bkt_array(unsigned int* bkt, size_t a, unsigned int pos) {
    printf("bkt: [");
    for (int i = 0; i < a; i++) {
        printf(" %3u,", bkt[i]);
    }
    printf("]\n      ");
    for (int i = 0; i < a; i++) {
        if (i == pos) {
            printf("  /\\ ");
        }
        printf("     ");
    }
    printf("\n");
}

void print_suffix_array(unsigned int* sa, size_t n, unsigned int pos) {
    printf("sa:  [");
    for (int i = 0; i < n; i++) {
        if (sa[i] == UINT_MAX) {
            printf("  - ,");
        } else {
            printf(" %3u,", sa[i]);
        }
    }
    printf("]\n      ");
    for (int i = 0; i < n; i++) {
        if (i == pos) {
            printf("  /\\ ");
        }
        printf("     ");
    }
    printf("\n");
}
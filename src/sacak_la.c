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

void print_bkt_array(unsigned int* bkt, size_t a, unsigned int pos) {
    printf("BKT: [");
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
    printf("SA:  [");
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

void print_lyndon_array(unsigned int* la, size_t n, unsigned int pos) {
    printf("LA:  [");
    for (int i = 0; i < n; i++) {
        if (la[i] == UINT_MAX) {
            printf("  - ,");
        } else {
            printf(" %3u,", la[i]);
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

void print_next_array(unsigned int* next, size_t n, unsigned int pos) {
    printf("NEXT:[");
    for (int i = 0; i < n; i++) {
        if (next[i] == UINT_MAX) {
            printf("  - ,");
        } else {
            printf(" %3u,", next[i]);
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

void print_prev_array(unsigned int* prev, size_t n, unsigned int pos) {
    printf("PREV:[");
    for (int i = 0; i < n; i++) {
        if (prev[i] == UINT_MAX) {
            printf(" -1 ,");
        } else {
            printf(" %3u,", prev[i]);
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

/*
*  Computes the bucket array for an string s
*  If head == 1, stores pointers to the head of the buckets, otherwise to the tails
*/ 
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
*  Recursively computes SA, intial call with depth=0, n1=0
*/
int sacak_rec(unsigned char* t, unsigned int* sa, unsigned int* bkt, unsigned char* alphabet, 
                size_t n, size_t a, int depth, size_t n1) {
        // Induced sorting of the LMS-type suffixes, algorithm published by Nong et al. in 
        // "Two efficient algorithms for linear time suffixarray construction" (access with IEEE)

        // In the steps, the constant UINT_MAX will be used to represent -1 which is equivalent
        // for int->uint: -1 = -1 mod UINT_MAX = UINT_MAX
        
        // step 1: induced sort lms-type suffixes
        // step 1.1: find all LMS-suffixes
        
        if (depth > 0) {
          //   
        } 

        // top-level call
        
        for(int i = 0; i < n; i++) {                                                                // initialize sa with zeroes
            sa[i] = UINT_MAX;
        }
        get_bkt_array(t, bkt, alphabet, n, a, 0);                                                   // get pointers to the end of the buckets 
        int cur_type = 0;                                                                           
        int last_type = 1;                                                                          // t[n] = $ is S-type
        sa[bkt[alphabet[(int) t[n-1]]]] = n-1;                                                      // no need to shift bkt pointer
        for (int i = n-2; i >= 0; i--) {                                                            // left to right scan of t                                                    
            cur_type = lex_compare_symbols(t[i], t[i+1]) == 0 ?                                     
                        (last_type ? 1 : 0) :                                                       // t[i] == t[i+1] -> depends on the type of t[i+1]
                        (lex_compare_symbols(t[i], t[i+1]) == 1 ? 0 : 1);                           // t[i] != t[i+1] -> direct comparison
            printf("%c is of type %c\n", t[i], cur_type ? 'S' : 'L');
            if (cur_type == 0 && last_type == 1) {                                                  // insert LMS-suffixes into their buckets in sa
                printf("%c LMS-type \n", t[i+1]);
                sa[bkt[alphabet[(int) t[i+1]]]] = i+1;                                              // alphabet[(int) t[i]] is the value if the symbol at t[i]
                bkt[alphabet[(int) t[i+1]]]--;                                                      // shift bucket tail pointer one position to the left
            }
            last_type = cur_type;
        }
        print_suffix_array(sa, n, -1);

        // step 2: induce L-type LMS-prefixes
        get_bkt_array(t, bkt, alphabet, n, a, 1);
        for (int i = 0; i < n; i++) {                                                               // left-to-right scan of t
            if (sa[i] != UINT_MAX && sa[i] > 0 && lex_compare_symbols(t[sa[i]-1], t[sa[i]]) > -1) { // t[sa[i]-1] is L-type if t[sa[i]-1] >= t[sa[i]] 
                                                                                                    // since only the L-type block left of an LMS-type symbol is considered here
                sa[bkt[alphabet[(int) t[sa[i]-1] ]]] = sa[i] - 1;                                   // insert the suffix t[sa[i]-1] into the head of its bucket
                bkt[alphabet[(int) t[sa[i]-1 ]]]++;                                                 // shift bucket head pointer one position to the right
                if (i > 0) {sa[i] = 0;}                                                             // dont delete sa[0] as it is LMS-type
                print_suffix_array(sa, n, i);
            }
        }

        // step 1.3: induce sort LMS-prefixes
        get_bkt_array(t, bkt, alphabet, n, a, 0);
        for (int i = n-1; i > 0; i--) {                                                            // right-to-left scan of t
            if (sa[i] != UINT_MAX && sa[i] > 0 && lex_compare_symbols(t[sa[i]-1], t[sa[i]]) < 1) {  // t[sa[i]-1] is s-type if t[sa[i]-1] <= t[sa[i]] 
                                                                                                    // since only the S-type block right of an LMS-type symbol is considered here
                sa[bkt[alphabet[(int) t[sa[i]-1] ]]] = sa[i] - 1;                                   // insert the suffix t[sa[i]-1] into the tail of its bucket
                bkt[alphabet[(int) t[sa[i]-1 ]]]--;                                                 // shift bucket tail pointer one position to the left
                sa[i] = 0;
                print_suffix_array(sa, n, i);
            }
        }

        // at this point SA[1..n] contains all sorted LMS-substrings (careful not LMS-suffixes)
        // rename every LMS-substring as an Integer and store the counter

        // put the LMS-substrings into SA[0..n1-1]
        for (int i = 0; i < n; i++) {
            if (sa[i] != 0 && i != n1) {                                                            // if sa[n1] = sa[i], skip over it
                sa[n1] = sa[i];
                sa[i] = 0;
                n1++;
            }
        }

        // compare the LMS-substrings, if they are different, the smaller ones get a smaller value
        // they are sorted but can still have the same value
        int last_lms_pos = 0;
        int last_lms_len = 1;                                                                       // $ is the smallest LMS-substring and has always length 1
        int name_offset = 0;        
        int name_counter = 0;
        int t1 = n1;                                                                                // beginning of the empty part of SA

        sa[t1] = 1;                                                                                 // $ is unique and can be set already

        // create buckets in SA[0..n1-1] which store the number of appearances of an LMS-substring
        for (int i = 1; i < n1; i++) {
            int cur_lms_pos = i;                                                                    // for clarity
            int cur_lms_len = n+1;
            for (int j = 0; j < n; j++) {                                                           // find the length of the current LMS-substring by searching for the next one
                if (sa[j] > sa[cur_lms_pos] && (sa[j] - sa[cur_lms_pos] + 1) < cur_lms_len) {                          
                    cur_lms_len = sa[j] - sa[cur_lms_pos] + 1;
                }
            }

            int is_different = 0;
            if (last_lms_len != cur_lms_len) {                                                      // both LMS-substrings are for sure different
                is_different = 1;
            } else {                                                                                // compare which one is lexicographically larger
                for (int j = 0; j < cur_lms_len; j++) {
                    if (sa[cur_lms_pos] + j > n-1 || sa[last_lms_pos] + j > n-1) {                  // if one of the LMS-substrings ends with $ the first one has lower value
                        is_different = 1;
                    }
                    if (t[sa[cur_lms_pos]] != t[sa[last_lms_pos]]) {                                // if the LMS-substrings differ in a position the first has lower value
                        is_different = 1;
                    }
                }
            }
            if (is_different) {                                                                     // create a new name
                name_counter = cur_lms_pos;                                                         // store the current position so it can be incremented if necessary
                sa[t1 + name_counter] = 1;                                                          // set the bucket to 1
                    
            } else {                                                                                // increment the last name
                sa[t1 + name_counter]++;                                                            // increment the bucket of the last name
            }
            sa[n1 + name_offset] = name_counter;                                                    // store the names in SA[n1..n-1], fits since n1 < n/2
            name_offset++;                                                                          
            last_lms_pos = cur_lms_pos;                                                             
            last_lms_len = cur_lms_len;
        }

        print_suffix_array(sa, n, n1);

        // create the reduced string t1 from the bucket indices of the names by reading them 
        // backwards from sa[n1..n1+name_offset] and using the reference to the buckets in sa[0..n1-1]
        int t1_tail = n-1;                                                                          // end of the reduced string has to be shifted to the left
        for (int i = name_offset; i > n1; i++) {                                                    // name_offset is the index of the last name
            
        }


}



unsigned int* sacak_la_two_aux(unsigned char* t, unsigned char* alphabet, size_t n, size_t a) {
        
        // Memory allocation
        unsigned int* sa = (unsigned int*) malloc(n * sizeof(unsigned int));
        unsigned int* la = (unsigned int*) malloc(n * sizeof(unsigned int));
        unsigned int* next = (unsigned int*) malloc(n * sizeof(unsigned int));                
        unsigned int* prev = (unsigned int*) malloc(n * sizeof(unsigned int));

        unsigned int* bkt = (unsigned int*) malloc(a * sizeof(unsigned int));                       // bkt has size sigma + 1, sigma < 255
        
        if (sa == 0 || la == 0 || next == 0 || prev == 0 || bkt == 0) {
            perror("malloc: ");
            printf("Could not allocate memory for la, sa, next, prev");
            exit(-1);
        }
        printf("Memory allocation alright");
        
        sacak_rec(t, sa, bkt, alphabet, n, a, 0, 0);

        for (int i = 0; i < n; i++) {
            next[i] = i+1;
            prev[i] = i-1;                                                                          // prev[0] = UINT_MAX
            la[i] = 0;                                                                              // not necessary
        }


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



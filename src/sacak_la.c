#include "sacak_la.h"

int debug = 0;
int output = 0;
int test = 0;

double get_timer() {
    return sum_time;
}

int start_timer() {
    start_time = clock();
    return 0;
}

int stop_timer() {
    /*
    if (start_time == (clock_t) 0) {
        printf("stop_time() call with no start_time() call before\n");
        return -1;
    }
    */
    clock_t end_time = clock();
    sum_time += (double) (end_time - start_time) / CLOCKS_PER_SEC;
    start_time = 0;
    return 0;
    
}

int reset_timer() {
    sum_time = 0;
    start_time = 0;
    return 0;
}

/*
*  TODO: Clean up all print functions, 
*/
void print_array(unsigned int* a, size_t len, unsigned int pos) {
    for (unsigned int i = 0; i < len; i++) {
        if (a[i] == UINT_MAX) {
            printf("  - ");
        } else {
            printf(" %3u", a[i]);
        }
        if (i < len-1) {
            printf(",");
        }
    }
    printf("]\n      ");
    if (pos == UINT_MAX) {
        return;
    }
    for (unsigned int i = 0; i < len; i++) {
        if (i == pos) {
            printf("  /\\ ");
        }
        printf("     ");
    }
    printf("\n");
}

void print_bkt_array(unsigned int* bkt, size_t a, unsigned int pos) {
    printf("BKT: [");
    print_array(bkt, a, pos);
}

void print_suffix_array(unsigned int* sa, size_t n, unsigned int pos) {
    printf("SA:  [");
    print_array(sa, n, pos);
}

void print_lyndon_array(unsigned int* la, size_t n, unsigned int pos) {
    printf("LA:  [");
    print_array(la, n, pos);
}

void print_next_array(unsigned int* next, size_t n, unsigned int pos) {
    printf("NEXT:[");
    print_array(next, n, pos);
}

void print_prev_array(unsigned int* prev, size_t n, unsigned int pos) {
    printf("PREV:[");
    print_array(prev, n, pos);
}

/*
*  Computes the bucket array for an string t
*  If head==1, stores pointers to the head of the buckets, otherwise to the tails
*  If rec==1, takes the entries of t as their lex. values
*/ 
void get_bkt_array(unsigned int* t, unsigned int* bkt, unsigned char* alphabet, size_t n, size_t a, int head, int rec) {
    for (unsigned int i = 0; i < a; i++) {
        bkt[i] = 0;
    }
    for (unsigned int i = 0; i < n; i++) {
        if (rec) {
            bkt[t[i]] += 1;                                                                         // in the recursion the value of a symbol is the symbol itself
        } else {
            bkt[alphabet[ t[i]]] += 1;                                                              // alphabet[(int) t[i]] is the value of the symbol at i,
        }
    }    
    unsigned int offset = 0;
    for (unsigned int i = 0; i < a; i++) {
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
int sacak_rec(unsigned int* t, unsigned int* sa, unsigned char* alphabet, 
                size_t n, size_t a, int depth) {
    // Induced sorting of the LMS-type suffixes, algorithm published by Nong et al. in 
    // "Two efficient algorithms for linear time suffixarray construction" (access with IEEE)

    // In the steps, the constant UINT_MAX will be used to represent -1 which is equivalent
    // for int->uint: -1 = -1 mod UINT_MAX = UINT_MAX
    
    if (depth > 0) {
        unsigned int* bkt = (unsigned int*) malloc(a * sizeof(unsigned int));                   
        if (bkt == 0) {
            perror("malloc: ");
            printf("Could not allocate memory for bkt\n");
            exit(-1);
        }
        if (_msize(bkt) > max_bkt_memory) {
            max_bkt_memory = _msize(bkt);
        }

        if (debug) { 
            printf("\nInduced sorting of LMS-substrings, depth=%u\nT%u := ", depth, depth); 
            print_suffix_array(t, n, 0);
            printf("0 is of type S");                                                               // always correct
        }

        for(unsigned int i = 0; i < n; i++) {                                                                // initialize sa with zeroes
            sa[i] = UINT_MAX;
        }
        get_bkt_array(t, bkt, alphabet, n, a, 0, 1);                                                // get pointers to the end of the buckets
        int cur_type = 0;                                                                           
        int last_type = 1;                                                                          // t[n] = $ is S-type
        //sa[bkt[ t[n-1] ]] = n-1;                                                                  // no need to shift bkt pointer
        for (unsigned int i = n-2; i != UINT_MAX; i--) {                                                            // left to right scan of t                                                    
            cur_type = lex_compare_symbols(t[i], t[i+1]) == 0 ?                                     
                        (last_type ? 1 : 0) :                                                       // t[i] == t[i+1] -> depends on the type of t[i+1]
                        (lex_compare_symbols(t[i], t[i+1]) == 1 ? 0 : 1);                           // t[i] != t[i+1] -> direct comparison     
            if (cur_type == 0 && last_type == 1) {                                                  // insert LMS-suffixes into their buckets in sa
                if (debug) {
                    printf("*");
                }
                sa[bkt[ t[i+1]]] = i+1;                                                             // alphabet[(int) t[i]] is the value if the symbol at t[i]
                bkt[ t[i+1]]--;                                                                     // shift bucket tail pointer one position to the left
            }
            if (debug) {
                printf("\n%u is of type %c", t[i], cur_type ? 'S' : 'L');
            }
            last_type = cur_type;
        }
        if (debug) {
            printf("\n\n");   
            print_suffix_array(sa, n, 0);    
        } 

        // step 1.2: induce L-type LMS-prefixes
        get_bkt_array(t, bkt, alphabet, n, a, 1, 1); 
        for (unsigned int i = 0; i < n; i++) {                                                               // left-to-right scan of t
            if (sa[i] == 0) { sa[i] = UINT_MAX; }
            if (sa[i] != UINT_MAX && lex_compare_symbols(t[sa[i]-1], t[sa[i]]) > -1) {              // t[sa[i]-1] is L-type if t[sa[i]-1] >= t[sa[i]] 
                                                                                                    // since only the L-type block left of an LMS-type symbol is considered here 
                if (sa[i]-1 != 0) {                                                                 // skipping zero since it cant be lms anyways                                                                                      
                    sa[bkt[ t[sa[i]-1] ]] = sa[i] - 1;                                              // insert the suffix t[sa[i]-1] into the head of its bucket
                    bkt[ t[sa[i]-1] ]++;                                                            // shift bucket head pointer one position to the right
                }
                if (i > 0) { sa[i] = UINT_MAX; }                                                    // dont delete sa[0] as it is LMS-type                                                          
                if (debug) {
                    print_suffix_array(sa, n, i);
                }
            }
        } 


        // step 1.3: induce S-type-prefixes
        get_bkt_array(t, bkt, alphabet, n, a, 0, 1);
        for (unsigned int i = n-1; i > 0; i--) {                                                             // right-to-left scan of t
            if (sa[i] == 0) { sa[i] = UINT_MAX; }
            if (sa[i] != UINT_MAX && lex_compare_symbols(t[sa[i]-1], t[sa[i]]) < 1) {               // t[sa[i]-1] is s-type if t[sa[i]-1] <= t[sa[i]] 
                                                                                                    // since only the S-type block right of an LMS-type symbol is considered here
                if (sa[i]-1 != 0) {                                                                 // skipping zero since it cant be lms anyways
                    sa[bkt[ t[sa[i]-1] ]] = sa[i] - 1;                                              // insert the suffix t[sa[i]-1] into the tail of its bucket
                    bkt[ t[sa[i]-1] ]--;                                                            // shift bucket tail pointer one position to the left
                }
                sa[i] = UINT_MAX;
                if (debug) {
                    print_suffix_array(sa, n, i);
                }
            }
        } 

        // at this point SA[1..n] contains all sorted LMS-substrings (careful not LMS-suffixes)
        // step 2: rename every LMS-substring as an Integer and store the counter

        // step 2.1: put the LMS-substrings into SA[0..n1-1]
        size_t n1 = 0;                                                                                 // n1 := |t1| -> #of LMs-substrngs in t
        for (unsigned int i = 0; i < n; i++) {
            if (sa[i] != UINT_MAX) {
                unsigned int temp = sa[i];
                sa[i] = UINT_MAX;
                sa[n1] = temp;
                n1++;
            }
        }

        if (debug) {
            print_suffix_array(sa, n, n1);
            printf("\nRenaming LMS-substrings\n");
        }

        // step 2.2: compare the LMS-substrings, if they are different, the smaller ones get a lower value
        unsigned int last_lms_pos = 0;                                                                       // int t not sa since that value will be  overidden !!!
        unsigned int last_lms_len = 0;  
        unsigned int name_counter = 0;

        // create buckets in SA[0..n1-1] which store the number of appearances of an LMS-substring
        for (unsigned int i = 0; i < n1; i++) {
            unsigned int cur_lms_pos = sa[i];                                                                // for clarity
            unsigned int cur_lms_len = 0;
           
            // get the length of the LMS-substring sa[i]
            // go to the right until t[sa[i] + j - 1] > t[sa[i] + j] -> len = j
            if (sa[i] != n-1) {                                                                         // LMS-substring $ has length 1

                while(sa[i] + cur_lms_len < n-1 && lex_compare_symbols(t[sa[i] + cur_lms_len], t[sa[i] + cur_lms_len + 1]) < 1) {    // S-type block to the right of the LMS-symbol
                    cur_lms_len++;
                }

                unsigned int last_switch = cur_lms_len;
                while(sa[i] + cur_lms_len < n-1 && lex_compare_symbols(t[sa[i] + cur_lms_len], t[sa[i] + cur_lms_len + 1]) > -1) {  // L-type block to the right of the LMS-symbol
                    if (lex_compare_symbols(t[sa[i] + cur_lms_len], t[sa[i] + cur_lms_len + 1]) > 0) {
                        last_switch = cur_lms_len;
                    }
                    cur_lms_len++;
                }
                cur_lms_len = last_switch;

                //while(lex_compare_symbols(t[sa[i] + cur_lms_len], t[sa[i] + cur_lms_len + 1]) < 1) {    // search for the next LMS-type-symbol
                //    cur_lms_len++;
                //}
                cur_lms_len++;                                                                          // include the next LMS-symbol as well
            } else {
                cur_lms_len = 1;
            } 
            //printf("LMS-SS at t[%u] has length %u", cur_lms_pos, cur_lms_len);
            int is_different = 0;
            if (last_lms_len != cur_lms_len) {                                                      // both LMS-substrings are for sure different
                is_different = 1;
            } else {                                                                                // compare if one is lexicographically larger
                for (unsigned int j = 0; j < cur_lms_len; j++) {
                    if (cur_lms_pos + j == n-1 || last_lms_pos + j == n-1) {                          // if one of the LMS-substrings ends with $ the first one has lower value
                        is_different = 1;
                        break;
                    }
                    if (t[cur_lms_pos + j] != t[last_lms_pos + j]) {                                // if the LMS-substrings differ in a position the first has lower value
                        //printf("Difference in t[%u] = %u != t[%u] = %u\n", cur_lms_pos + j, t[cur_lms_pos + j], last_lms_pos + j, t[last_lms_pos + j]);
                        is_different = 1;
                        break;
                    }
                }
            } 


            if (is_different) {                                                                     // create a new name
                name_counter++;                                                                     // store the current position so it can be incremented if necessary
                last_lms_pos = cur_lms_pos;                                                             
                last_lms_len = cur_lms_len;    
            } //printf(",   name assigned: %u different?: %u\n", name_counter-1, is_different);
            sa[n1 + ((cur_lms_pos % 2 == 0) ? (cur_lms_pos / 2) : (cur_lms_pos - 1) / 2)] = name_counter - 1;   // store the names in SA[n1..n-1], fits since n1 < n/2   
            if (debug) {
                print_suffix_array(sa, n, n1 + ((cur_lms_pos % 2 == 0) ? (cur_lms_pos / 2) : (cur_lms_pos - 1) / 2));
            }                                                                   
        } 
        
        // put the reduced string to the end of sa
        unsigned int j = n - 1;
        for (unsigned int i = n - 1; i >= n1; i--) {
            if (sa[i] != UINT_MAX) {
                unsigned int temp = sa[i];
                sa[i] = UINT_MAX;
                sa[j] = temp;
                j--;
            }
        }

        if (debug) {
                print_suffix_array(sa, n, n - n1);
        }
        
        //printf("\n\nDepth: %u\n", depth);
        //print_suffix_array(t, n, -1);
        //print_suffix_array(sa, n, -1);


        // step 2.3: test if every character in t1 is unique
        int is_unique = (name_counter < n1) ? 0 : 1;

        if (debug) {
            printf(is_unique ? "Every character unique\n" : "Recursion:\n");
        }

        unsigned int* t1 = sa + n - n1;
        unsigned int* sa1 = sa;
        int depthr = 0;
        //if (depth == 6) print_suffix_array(t1, n1, -1);
        if (is_unique) {
            // TODO: Get sa1 directly from t1 
            for (unsigned int i = 0; i < n1; i++) {                                                      
                sa1[t1[i]] = i;
            }
            if (debug) {
                print_suffix_array(sa, n, n1);
            }
        } else {
            // TODO: Get sa1 by recursion with sa1 := sa[0..n-n1-1], t1 := sa[n-n1-1..n-1]
            free(bkt);
            depthr = sacak_rec(t1, sa1, alphabet, n1, name_counter, depth + 1);                     // n-n1 =: length of sa1, n1 =: length of t1
            
            // Get the bucket array back
            bkt = (unsigned int*) malloc(a * sizeof(unsigned int));                   
                if (bkt == 0) {
                perror("malloc: ");
                printf("Could not allocate memory for bkt\n");
                exit(-1);
            }
            if (_msize(bkt) > max_bkt_memory) {
                max_bkt_memory = _msize(bkt);
            }
        }
        // suffix array of sa1 is now in sa[0..n1-1]

        //printf("\n\nDepth: %u\n", depth);
        //print_suffix_array(t, n, -1);
        //print_suffix_array(sa, n, -1);

        /*
        // check if the suffix array is correct
        for (unsigned int i = 1; i < n1; i++) {
            for (unsigned int j = sa1[i], k = sa1[i-1]; j < n1 && k < n1; j++, k++) {
                if (t1[j] > t1[k]) {
                    break;
                }
                if (t1[j] < t1[k]) {
                    printf("Suffix array incorrect:\n");
                    printf("%u: t[%u] = %u < t[%u] = %u, beginning at %u, %u\n", i, j, t1[j], k, t1[k], sa1[i], sa1[i-1]);
                    exit(-1);
                }
            }
        }
        printf("Suffix array correct\n");
        */

        // step 3: induce sa from sa1
        // find pointers to the LMS-factors in t and store them in t1[0..n1-1]
        get_bkt_array(t, bkt, alphabet, n, a, 0, 1);                                                // get pointers to the end of the buckets 
        for (unsigned int i = 0; i < n1; i++) t1[i] = UINT_MAX;                                              // clean t1
        j = n1-1;
        cur_type = 0;                                                                           
        last_type = 1;                                                                              // t[n] = $ is S-type
        for (unsigned int i = n-2; i != UINT_MAX; i--) {                                                            // left to right scan of t                                                    
            cur_type = lex_compare_symbols(t[i], t[i+1]) == 0 ?                                     
                        (last_type ? 1 : 0) :                                                       // t[i] == t[i+1] -> depends on the type of t[i+1]
                        (lex_compare_symbols(t[i], t[i+1]) == 1 ? 0 : 1);                           // t[i] != t[i+1] -> direct comparison     

            if (cur_type == 0 && last_type == 1) {                                                  // insert LMS-suffixes into their buckets in sa
                t1[j] = i+1;
                j--;
            }
            last_type = cur_type;
        }
        if (debug) {
            print_suffix_array(sa, n, n - n1 + j);
        }

        // order the lms-suffixes using sa1
        for (unsigned int i = 0; i < n1; i++) {
            sa1[i] = t1[sa1[i]];
        }
        if (debug) {
            print_suffix_array(sa, n, n1);
        }

        // clean rest of sa
        for (unsigned int i = n1; i < n; i++) {
            sa[i] = UINT_MAX;
        }
        if (debug) {
            print_suffix_array(sa, n, n1);
        }

        // put the ordered LMS-substrings from sa1 into the ends of their buckets in sa
        for (unsigned int i = n1-1; i != UINT_MAX; i--) {
            j = sa[i];                                                                              // tmp var in case sa[i] = sa[bkt[t[sa[i]]]]
            sa[i] = UINT_MAX;
            sa[bkt[ t[j] ]] = j;
            bkt[ t[j] ]--;
        }
        if (debug) {
            print_suffix_array(sa, n, n1);
        }

        // step 3.2: Induce L-type suffixes from the ordered LMS-type-suffixes, for comments look step 1.2
        get_bkt_array(t, bkt, alphabet, n, a, 1, 1);
        for (unsigned int i = 0; i < n; i++) {                                                               
            if (sa[i] != UINT_MAX && sa[i] > 0 && lex_compare_symbols(t[sa[i]-1], t[sa[i]]) > -1) {
                sa[bkt[ t[sa[i]-1] ]] = sa[i] - 1;
                bkt[ t[sa[i]-1 ]]++;
                if (debug) {
                    print_suffix_array(sa, n, i);
                }
            }
        }
        // step 3.3: Induce S-type suffixes
        get_bkt_array(t, bkt, alphabet, n, a, 0, 1);
        for (unsigned int i = n-1; i != UINT_MAX; i--) {                                                             // right-to-left scan of t
            if (sa[i] != UINT_MAX && sa[i] > 0 && lex_compare_symbols(t[sa[i]-1], t[sa[i]]) < 1) {  // t[sa[i]-1] is s-type if t[sa[i]-1] <= t[sa[i]] 
                                                                                                    // since only the S-type block right of an LMS-type symbol is considered here
                sa[bkt[ t[sa[i]-1] ]] = sa[i] - 1;                                                  // insert the suffix t[sa[i]-1] into the tail of its bucket
                bkt[ t[sa[i]-1 ]]--;                                                                // shift bucket tail pointer one position to the left
                if (debug) {
                    print_suffix_array(sa, n, i);
                }
            }
        }
        free(bkt);
        return (depthr > depth ? depthr : depth);
    } 








    // top-level call

    // Get the bucket array
    unsigned int* bkt = (unsigned int*) malloc(a * sizeof(unsigned int));                   
        if (bkt == 0) {
        perror("malloc: ");
        printf("Could not allocate memory for bkt\n");
        exit(-1);
    }
    if (_msize(bkt) > max_bkt_memory) {
        max_bkt_memory = _msize(bkt);
    }

    // step 1: induced sort lms-type suffixes
    // step 1.1: find all LMS-suffixes

    if (debug) { 
        printf("\nInduced sorting of LMS-substrings, depth=0\n"); 
        printf("$ is of type S");                                                                   // always correct
    }

    for(unsigned int i = 0; i < n; i++) {                                                           // initialize sa with zeroes
        sa[i] = UINT_MAX;
    }
    get_bkt_array(t, bkt, alphabet, n, a, 0, 0);                                                    // get pointers to the end of the buckets 
    int cur_type = 0;
    int last_type = 1;                                                                              // t[n] = $ is S-type
    //sa[bkt[alphabet[ t[n-1] ]]] = n-1;                                                            // no need to shift bkt pointer
    for (unsigned int i = n-2; i != UINT_MAX; i--) {                                                                // left to right scan of t                                                    
        cur_type = lex_compare_symbols(t[i], t[i+1]) == 0 ?                                     
                    (last_type ? 1 : 0) :                                                           // t[i] == t[i+1] -> depends on the type of t[i+1]
                    (lex_compare_symbols(t[i], t[i+1]) == 1 ? 0 : 1);                               // t[i] != t[i+1] -> direct comparison     

        if (cur_type == 0 && last_type == 1) {                                                      // insert LMS-suffixes into their buckets in sa
            if (debug) {
                printf("*");
            }
            sa[bkt[alphabet[ t[i+1]]]] = i+1;                                                       // alphabet[(int) t[i]] is the value if the symbol at t[i]
            bkt[alphabet[ t[i+1]]]--;                                                               // shift bucket tail pointer one position to the left
            
        }
        if (debug) {
            printf("\n%c is of type %c", t[i], cur_type ? 'S' : 'L');
        }
        last_type = cur_type;
    }
    if (debug) {
        printf("\n\n"); 
        print_suffix_array(sa, n, 0);      
    }

    // step 1.2: induce L-type LMS-prefixes
    get_bkt_array(t, bkt, alphabet, n, a, 1, 0);
    for (unsigned int i = 0; i < n; i++) {                                                                   // left-to-right scan of t
        if (sa[i] != UINT_MAX && lex_compare_symbols(t[sa[i]-1], t[sa[i]]) > -1) { // t[sa[i]-1] is L-type if t[sa[i]-1] >= t[sa[i]] 
                                                                                                    // since only the L-type block left of an LMS-type symbol is considered here
            if (sa[i]-1 != 0) {                                                                     // skipping zero since it cant be lms anyways
                sa[bkt[alphabet[ t[sa[i]-1] ]]] = sa[i] - 1;                                        // insert the suffix t[sa[i]-1] into the head of its bucket
                bkt[alphabet[ t[sa[i]-1 ]]]++;                                                      // shift bucket head pointer one position to the right
            }
            if (i > 0) {sa[i] = UINT_MAX;}                                                          // dont delete sa[0] as it is LMS-type
            if (debug) {
                print_suffix_array(sa, n, i);
            }
        }
    }

    // step 1.3: induce S-type-prefixes
    get_bkt_array(t, bkt, alphabet, n, a, 0, 0);
    for (unsigned int i = n-1; i > 0; i--) {                                                                 // right-to-left scan of t
        if (sa[i] != UINT_MAX && lex_compare_symbols(t[sa[i]-1], t[sa[i]]) < 1) {                   // t[sa[i]-1] is s-type if t[sa[i]-1] <= t[sa[i]] 
                                                                                                    // since only the S-type block right of an LMS-type symbol is considered here
            if (sa[i]-1 != 0) {                                                                     // skipping zero since it cant be lms anyways
                sa[bkt[alphabet[ t[sa[i]-1] ]]] = sa[i] - 1;                                        // insert the suffix t[sa[i]-1] into the tail of its bucket
                bkt[alphabet[ t[sa[i]-1 ]]]--;                                                      // shift bucket tail pointer one position to the left
            }
            sa[i] = UINT_MAX;
            if (debug) {
                print_suffix_array(sa, n, i);
            }
        }
    }

    // at this point SA[1..n] contains all sorted LMS-substrings (careful not LMS-suffixes)
    // step 2: rename every LMS-substring as an Integer and store the counter

    // step 2.1: put the LMS-substrings into SA[0..n1-1]
    size_t n1 = 0;                                                                                     // n1 := |t1| -> #of LMs-substrngs in t
    for (unsigned int i = 0; i < n; i++) {
        if (sa[i] != UINT_MAX) {
            unsigned int temp = sa[i];
            sa[i] = UINT_MAX;
            sa[n1] = temp;
            n1++;
        }
    }

    if (debug) {
        print_suffix_array(sa, n, n1);
        printf("\nRenaming LMS-substrings\n");
    }

    // step 2.2: compare the LMS-substrings, if they are different, the smaller ones get a lower value
    unsigned int last_lms_pos = 0;                                                                           // in t not sa since that value will be overidden !!!
    unsigned int last_lms_len = 0;     
    unsigned int name_counter = 0;

    for (int i = 0; i < n1; i++) {
        unsigned int cur_lms_pos = sa[i];                                                                    // for clarity
        unsigned int cur_lms_len = 0;

        // get the length of the LMS-substring sa[i]
        if (sa[i] != n-1) {                                                                         // LMS-substring $ has length 1
            while(sa[i] + cur_lms_len < n-1 && lex_compare_symbols(t[sa[i] + cur_lms_len], t[sa[i] + cur_lms_len + 1]) < 1) {    // S-type block to the right of the LMS-symbol
                cur_lms_len++;
            }

            unsigned int last_switch = cur_lms_len;
            while(sa[i] + cur_lms_len < n-1 && lex_compare_symbols(t[sa[i] + cur_lms_len], t[sa[i] + cur_lms_len + 1]) > -1) {  // L-type block to the right of the LMS-symbol
                if (lex_compare_symbols(t[sa[i] + cur_lms_len], t[sa[i] + cur_lms_len + 1]) > 0) {
                    last_switch = cur_lms_len;
                }
                cur_lms_len++;
            }
            cur_lms_len = last_switch;

            //while(lex_compare_symbols(t[sa[i] + cur_lms_len], t[sa[i] + cur_lms_len + 1]) < 1) {    // search for the next LMS-type-symbol
            //    cur_lms_len++;
            //}
            cur_lms_len++;                                                                          // include the next LMS-symbol as well
        } else {
            cur_lms_len = 1;
        }
        //printf("LMS-SS at t[%u] has length %u", cur_lms_pos, cur_lms_len);
        int is_different = 0;
        if (last_lms_len != cur_lms_len) {                                                          // both LMS-substrings are for sure different
            is_different = 1;
        } else {                                                                                    // compare which one is lexicographically larger
            for (unsigned int j = 0; j < cur_lms_len; j++) {
                if (cur_lms_pos + j == n-1 || last_lms_pos + j == n-1) {                              // if one of the LMS-substrings ends with $ the first one has lower value
                    is_different = 1;
                    break;
                }
                if (t[cur_lms_pos + j] != t[last_lms_pos + j]) {                                    // if the LMS-substrings differ in a position the first has lower value
                    //printf("Difference in t[%u] = %u != t[%u] = %u\n", cur_lms_pos + j, t[cur_lms_pos + j], last_lms_pos + j, t[last_lms_pos + j]);
                    is_different = 1;
                    break;
                }
            }
        }

        if (is_different) {                                                                         // create a new name
            name_counter++;                                                                         // store the current position so it can be incremented if necessary
            last_lms_pos = cur_lms_pos;                                                             
            last_lms_len = cur_lms_len;  
        } //printf(",   name assigned: %u\n", name_counter-1);
        sa[n1 + ((cur_lms_pos % 2 == 0) ? (cur_lms_pos / 2) : (cur_lms_pos - 1) / 2)] = name_counter - 1;   // store the names in SA[n1..n-1], fits since n1 < n/2     
        if (debug) {
            print_suffix_array(sa, n, n1 + ((cur_lms_pos % 2 == 0) ? (cur_lms_pos / 2) : (cur_lms_pos - 1) / 2));
        }                                                                   
    }

    // put the reduced string to the end of sa
    unsigned int j = n - 1;
    for (unsigned int i = n - 1; i >= n1; i--) {                                                         
        if (sa[i] != UINT_MAX) {
            unsigned int temp = sa[i];
            sa[i] = UINT_MAX;
            sa[j] = temp;
            j--;
        }
    }

        //printf("\n\nDepth: %u\n", depth);
        //print_suffix_array(t, n, -1);
        //print_suffix_array(sa, n, -1);

    if (debug) {
        print_suffix_array(sa, n, n - n1);
    }

    // step 2.3: test if every character in t1 is unique
    int is_unique = (name_counter < n1) ? 0 : 1;

    if (debug) {
        printf(is_unique ? "Every character unique\n" : "Recursion:\n");
    }

    unsigned int* t1 = sa + n - n1;
    unsigned int* sa1 = sa;
    int depthr = 0;
    
    if (is_unique) {
        // TODO: Get sa1 directly from t1 
        for (unsigned int i = 0; i < n1; i++) {                                                          
            sa1[t1[i]] = i;
        }
        if (debug) {
            print_suffix_array(sa, n, n1-1);
        }
    } else {
        // TODO: Get sa1 by recursion with sa1 := sa[0..n-n1-1], t1 := sa[n-n1-1..n-1]
        free(bkt);
        int temp = ascii;
        ascii = 1;                                                                                  // dont use the alphabet conversion for this part
        depthr = sacak_rec(t1, sa1, alphabet, n1, name_counter, depth + 1);                         // n1 =: length of sa1, n1 =: length of t1
        ascii = temp;                                                                               // enable it again
        
        // Get the bucket array back
        bkt = (unsigned int*) malloc(a * sizeof(unsigned int));                   
            if (bkt == 0) {
            perror("malloc: ");
            printf("Could not allocate memory for bkt\n");
            exit(-1);
        }
        if (_msize(bkt) > max_bkt_memory) {
            max_bkt_memory = _msize(bkt);
        }
    }
    // suffix array of sa1 is now in sa[0..n1]

        //printf("\n\nDepth: %u\n", depth);
        //print_suffix_array(t, n, -1);
        //print_suffix_array(sa, n, -1);

    // step 3: induce sa from sa1
    // find pointers to the LMS-substrings in t1 and store them in s1[0..n1]
    get_bkt_array(t, bkt, alphabet, n, a, 0, 0);                                                    // get pointers to the end of the buckets 
    for (unsigned int i = 0; i < n1; i++) t1[i] = UINT_MAX;                                         // clean t1

    j = n1-1;
    cur_type = 0;                                                                           
    last_type = 1;                                                                                  // t[n] = $ is S-type
    for (unsigned int i = n-2; i != UINT_MAX; i--) {                                                                // left to right scan of t                                                    
        cur_type = lex_compare_symbols(t[i], t[i+1]) == 0 ?                                     
                    (last_type ? 1 : 0) :                                                           // t[i] == t[i+1] -> depends on the type of t[i+1]
                    (lex_compare_symbols(t[i], t[i+1]) == 1 ? 0 : 1);                               // t[i] != t[i+1] -> direct comparison     

        if (cur_type == 0 && last_type == 1) {                                                      // insert LMS-suffixes into their buckets in sa
            t1[j] = i+1;
            j--;
        }
        last_type = cur_type;
    }
    if (debug) {
        print_suffix_array(sa, n, n - n1 + j);
    }
    // order the lms-suffixes using sa1
    for (unsigned int i = 0; i < n1; i++) {
        sa1[i] = t1[sa1[i]];
    }
    if (debug) {
        print_suffix_array(sa, n, n1);
    }
    // clean rest of sa
    for (unsigned int i = n1; i < n; i++) {
        sa[i] = UINT_MAX;
    }
    if (debug) {
        print_suffix_array(sa, n, n1);
        //print_bkt_array(bkt, a, -1);
    }

    // put the ordered LMS-substrings into the ends of their bucket
    for (unsigned int i = n1-1; i != UINT_MAX; i--) {
        j = sa[i];
        sa[i] = UINT_MAX;
        sa[bkt[alphabet[ t[j] ]]] = j;
        bkt[alphabet[ t[j] ]]--;
        if (debug) {
            print_suffix_array(sa, n, n1);
        }
    }

    // step 3.2: Induce L-type suffixes from the ordered LMS-type-suffixes, for comments look step 1.2
    get_bkt_array(t, bkt, alphabet, n, a, 1, 0);
    for (unsigned int i = 0; i < n; i++) {                                                          
        if (sa[i] != UINT_MAX && sa[i] > 0 && lex_compare_symbols(t[sa[i]-1], t[sa[i]]) > -1) {
            sa[bkt[alphabet[ t[sa[i]-1] ]]] = sa[i] - 1;
            bkt[alphabet[ t[sa[i]-1 ]]]++;
            if (debug) {
                print_suffix_array(sa, n, i);
            }
        }
    }

    // step 3.3 is in the calling method
    free(bkt); printf("SACA-K done \n");
    // return max recursion depth
    return (depthr != 0 ? depthr : 0);
}











void sacak(unsigned int* t, unsigned char* alphabet, size_t n, size_t a) {
    // Memory allocation
    unsigned int* sa = (unsigned int*) malloc(n * sizeof(unsigned int));

    if (sa == 0) {
        perror("malloc: ");
        printf("Could not allocate memory for sa\n");
        exit(-1);
    }

    reset_timer();
    start_timer();

    int depthr = sacak_rec(t, sa, alphabet, n, a, 0);

    unsigned int* bkt = (unsigned int*) malloc(a * sizeof(unsigned int));                   
    if (bkt == 0) {
        perror("malloc: ");
        printf("Could not allocate memory for bkt\n");
        exit(-1);
    }

    max_memory = 0;
    max_bkt_memory = 0;
    max_memory += _msize(sa);
    max_alphabet_memory = alphabet == NULL ? 0 : _msize(alphabet);
    if (_msize(bkt) > max_bkt_memory) {
        max_bkt_memory = _msize(bkt);
    }

    get_bkt_array(t, bkt, alphabet, n, a, 0, 0);
    for (unsigned int i = n-1; i != UINT_MAX; i--) {                                                                // right-to-left scan of t
        if (sa[i] != UINT_MAX && sa[i] > 0 && lex_compare_symbols(t[sa[i]-1], t[sa[i]]) < 1) {      // t[sa[i]-1] is s-type if t[sa[i]-1] <= t[sa[i]] 
            sa[bkt[alphabet[ t[sa[i]-1] ]]] = sa[i] - 1;                                            // insert the suffix t[sa[i]-1] into the tail of its bucket
            bkt[alphabet[ t[sa[i]-1 ] ]]--;                                                         // shift bucket tail pointer one position to the left
        }
        if (debug) {
            print_suffix_array(sa, n, i);
        }
    }

    stop_timer();

    // output the result
    if (output) {
        print_suffix_array(sa, n, -1);
    }

    if (output) {
        printf("Max. recursion depth=%i\n", depthr);
    }

    if (test) {
        int test_res = test_suffix_array(t, sa, alphabet, n, a);
        printf(test_res ? "Suffix array is correct\n" : "Suffix array is incorrect\n");
    }

    printf("Working space: %zu bytes + %zu bytes (bkt-array) + %zu bytes (alphabet)\n", max_memory, max_bkt_memory, max_alphabet_memory);
    printf("Working space per input byte: %zu \n", (max_memory + max_bkt_memory + max_alphabet_memory) / (size_t) n);

    // Free memory
    free(sa);

    sa = NULL;
    bkt = NULL;
}


unsigned int* sacak_la_two_aux(unsigned int* t, unsigned char* alphabet, size_t n, size_t a) {
   
    // Memory allocation
    unsigned int* sa = (unsigned int*) malloc(n * sizeof(unsigned int));
    unsigned int* la = (unsigned int*) malloc(n * sizeof(unsigned int));
    unsigned int* next = (unsigned int*) malloc(n * sizeof(unsigned int));                
    unsigned int* prev = (unsigned int*) malloc(n * sizeof(unsigned int));
    
    if (sa == 0 || la == 0 || next == 0 || prev == 0) {
        perror("malloc: ");
        printf("Could not allocate memory for la, sa, next, prev\n");
        exit(-1);
    }

    reset_timer();
    start_timer();

    int depthr = sacak_rec(t, sa, alphabet, n, a, 0);

    unsigned int* bkt = (unsigned int*) malloc(a * sizeof(unsigned int));                   
    if (bkt == 0) {
        perror("malloc: ");
        printf("Could not allocate memory for bkt\n");
        exit(-1);
    }

    max_memory = 0;
    max_bkt_memory = 0;
    max_memory += _msize(sa) + _msize(la) + _msize(next) + _msize(prev);
    max_alphabet_memory = alphabet == NULL ? 0 : _msize(alphabet);
    if (_msize(bkt) > max_bkt_memory) {
        max_bkt_memory = _msize(bkt);
    }

    // initialize the next/prev/la-array
    for (unsigned int i = 0; i < n; i++) {
        next[i] = i+1;
        prev[i] = i-1;                                                                              // prev[0] = UINT_MAX
        la[i] = 0;                                                                                  // not necessary
    }

    // step 3.3: Induce S-type suffixes and LA
    if (debug) {
        printf("Inducing Lyndon Array:\n");
    }
    get_bkt_array(t, bkt, alphabet, n, a, 0, 0);
    for (unsigned int i = n-1; i != UINT_MAX; i--) {                                                                // right-to-left scan of t
        if (sa[i] != UINT_MAX && sa[i] > 0 && lex_compare_symbols(t[sa[i]-1], t[sa[i]]) < 1) {      // t[sa[i]-1] is s-type if t[sa[i]-1] <= t[sa[i]] 
            sa[bkt[alphabet[ t[sa[i]-1] ]]] = sa[i] - 1;                                            // insert the suffix t[sa[i]-1] into the tail of its bucket
            bkt[alphabet[ t[sa[i]-1 ] ]]--;                                                         // shift bucket tail pointer one position to the left
        }

        unsigned int j = sa[i];
        la[j] = next[j] - j;
        if (prev[j] >= 0 && prev[j] < n) {
            next[prev[j]] = next[j];
        }
        if (next[j] >= 0 && next[j] < n) {
            prev[next[j]] = prev[j];
        }
        if (j == n-1) {
            la[n-1] = 1; 
        }
        if (debug) {
            print_suffix_array(sa, n, i);
            print_next_array(next, n, prev[j] > 0 ? prev[j] : -1);
            print_prev_array(prev, n, next[j] < n + 1 ? next[j] : -1);
            print_lyndon_array(la, n, j);
            printf("\n");
        }
    }                                                                                               // set the entry for the termination symbol


    stop_timer();

    // output the result
    if (output) {
        print_suffix_array(sa, n, -1);
        print_lyndon_array(la, n, -1);
    }

    if (output) {
        printf("Max. recursion depth=%i\n", depthr);
    }

    if (test) {
        int test_res = test_suffix_array(t, sa, alphabet, n, a);
        printf(test_res ? "Suffix array is correct\n" : "Suffix array is incorrect\n");
        test_res = test_lyndon_array(t, la, alphabet, n, a);                                              
        printf(test_res ? "Lyndon array is correct\n" : "Lyndon array is incorrect\n");
    }

    printf("Working space: %zu bytes + %zu bytes (bkt-array) + %zu bytes (alphabet)\n", max_memory, max_bkt_memory, max_alphabet_memory);
    printf("Working space per input byte: %zu \n", (max_memory + max_bkt_memory + max_alphabet_memory) / (size_t) n);

    // Free memory
    free(prev);
    free(next);
    free(sa);

    sa = NULL;
    next = NULL;
    prev = NULL;
    bkt = NULL;
    return la;
}

unsigned int* sacak_la_one_aux(unsigned int* t, unsigned char* alphabet, size_t n, size_t a) {
    
    // Memory allocation
    unsigned int* sa = (unsigned int*) malloc(n * sizeof(unsigned int));
    unsigned int* la = (unsigned int*) malloc(n * sizeof(unsigned int));
    unsigned int* h = (unsigned int*) malloc(n * sizeof(unsigned int));                             // Using h istead of a for the combined array as the variable is used already    
    
    if (sa == 0 || la == 0 || h == 0) {
        perror("malloc: ");
        printf("Could not allocate memory for la, sa, h\n");
        exit(-1);
    }

    reset_timer();
    start_timer();

    int depthr = sacak_rec(t, sa, alphabet, n, a, 0);

    unsigned int* bkt = (unsigned int*) malloc(a * sizeof(unsigned int));                   
    if (bkt == 0) {
        perror("malloc: ");
        printf("Could not allocate memory for bkt\n");
        exit(-1);
    }

    max_memory = 0;
    max_bkt_memory = 0;
    max_memory += _msize(sa) + _msize(la) + _msize(h);
    max_alphabet_memory = alphabet == NULL ? 0 : _msize(alphabet);
    if (_msize(bkt) > max_bkt_memory) {
        max_bkt_memory = _msize(bkt);
    }
    
    // initialize the h/la-array
    for (unsigned int i = 0; i < n; i++) {
        h[i] = i+1;                                                                                 // h initially stores the next array
        la[i] = 0;                                                                              
    }

    // step 3.3: Induce S-type suffixes and LA
    if (debug) {
        printf("Inducing Lyndon Array:\n");
    }
    get_bkt_array(t, bkt, alphabet, n, a, 0, 0);
    for (unsigned int i = n-1; i != UINT_MAX; i--) {                                                                // right-to-left scan of t
        if (sa[i] != UINT_MAX && sa[i] > 0 && lex_compare_symbols(t[sa[i]-1], t[sa[i]]) < 1) {      // t[sa[i]-1] is s-type if t[sa[i]-1] <= t[sa[i]] 
            sa[bkt[alphabet[ t[sa[i]-1] ]]] = sa[i] - 1;                                            // insert the suffix t[sa[i]-1] into the tail of its bucket
            bkt[alphabet[ t[sa[i]-1 ] ]]--;                                                         // shift bucket tail pointer one position to the left
        }

        unsigned int j = sa[i];
        la[j] = h[j] - j;
        if (j > 0) {
            unsigned int prev = la[j-1] == 0 ? j - 1 : h[j - 1];
            unsigned int next = h[j];
            h[next - 1] = prev;
            if (prev != UINT_MAX) {                                                                        // dont set the next value if prev = -1
                h[prev] = next;
            }
        } else {                                                                                    // only need to set the prev info to -1
            h[j] = UINT_MAX;
        }

        
        if (debug) {
            print_suffix_array(sa, n, i);
            print_next_array(h, n, h[j] > 0 ? h[j - 1] : -1);
            print_lyndon_array(la, n, j);
            printf("\n");
        }
    }                                                                                               // set the entry for the termination symbol


    stop_timer();

    // output the result
    if (output) {
        print_suffix_array(sa, n, -1);
        print_lyndon_array(la, n, -1);
    }

    if (output) {
        printf("Max. recursion depth=%i\n", depthr);
    }

    if (test) {
        int test_res = test_suffix_array(t, sa, alphabet, n, a);
        printf(test_res ? "Suffix array is correct\n" : "Suffix array is incorrect\n");
        test_res = test_lyndon_array(t, la, alphabet, n, a);                                              
        printf(test_res ? "Lyndon array is correct\n" : "Lyndon array is incorrect\n");
    }

    printf("Working space: %zu bytes + %zu bytes (bkt-array) + %zu bytes (alphabet)\n", max_memory, max_bkt_memory, max_alphabet_memory);
    printf("Working space per input byte: %zu \n", (max_memory + max_bkt_memory + max_alphabet_memory) / (size_t) n);

    // Free memory
    free(bkt);
    free(h);
    free(sa);

    sa = NULL;
    h = NULL;
    bkt = NULL;
    return la;
}



unsigned int* sacak_la_inplace(unsigned int* t, unsigned char* alphabet, size_t n, size_t a) { 
    // Memory allocation
    unsigned int* sa = (unsigned int*) malloc(n * sizeof(unsigned int));
    unsigned int* la = (unsigned int*) malloc(n * sizeof(unsigned int));           
    
    if (sa == 0 || la == 0) {
        perror("malloc: ");
        printf("Could not allocate memory for la, sa\n");
        exit(-1);
    }

    reset_timer();
    start_timer();

    int depthr = sacak_rec(t, sa, alphabet, n, a, 0);

    unsigned int* bkt = (unsigned int*) malloc(a * sizeof(unsigned int));                   
    if (bkt == 0) {
        perror("malloc: ");
        printf("Could not allocate memory for bkt\n");
        exit(-1);
    } 

    max_memory = 0;
    max_bkt_memory = 0;
    max_memory += _msize(sa) + _msize(la);
    max_alphabet_memory = alphabet == NULL ? 0 : _msize(alphabet);
    if (_msize(bkt) > max_bkt_memory) {
        max_bkt_memory = _msize(bkt);
    }
    
    // initialize the la-array
    for (unsigned int i = 0; i < n; i++) {
        la[i] = i+1;                                                                                // initialize like the next-array                                                                         
    }

    // step 3.3: Induce S-type suffixes and LA
    if (debug) {
        printf("Inducing Lyndon Array:\n");
    }
    get_bkt_array(t, bkt, alphabet, n, a, 0, 0);
    for (unsigned int i = n-1; i != UINT_MAX; i--) {                                                                // right-to-left scan of t
        if (sa[i] != UINT_MAX && sa[i] > 0 && lex_compare_symbols(t[sa[i]-1], t[sa[i]]) < 1) {      // t[sa[i]-1] is s-type if t[sa[i]-1] <= t[sa[i]] 
            sa[bkt[alphabet[ t[sa[i]-1] ]]] = sa[i] - 1;                                            // insert the suffix t[sa[i]-1] into the tail of its bucket
            bkt[alphabet[ t[sa[i]-1 ] ]]--;                                                         // shift bucket tail pointer one position to the left
        }

        unsigned int j = sa[i];
        if (j > 0) {                                                                                // identical to the version with one auxiliary array
            unsigned int prev = (la[j-1] < j || la[j-1] == UINT_MAX) ? la[j-1] : j - 1;
            unsigned int next = la[j];
            la[next - 1] = prev;
            if (prev != UINT_MAX) {                                                                 // dont set the next-value for prev = -1
                la[prev] = next;
            }
        } else {
            la[la[j] - 1] = UINT_MAX;                                                               // manually set the value if sa[i] = 0
        }
        
        if (debug) {
            print_suffix_array(sa, n, i);
            print_lyndon_array(la, n, j);
            printf("\n");
        }
    }                                                                                               // set the entry for the termination symbol

    for (unsigned int i = 0; i < n; i++) {                                                                   // single scan to compute la
        la[i] = (la[i] < i || la[i] == UINT_MAX) ? 1 : la[i] - i;
        if (debug) {
            print_lyndon_array(la, n, i);
        }
    }

    stop_timer();

    // output the result
    if (output) {
        print_suffix_array(sa, n, -1);
        print_lyndon_array(la, n, -1);
    }

    if (output) {
        printf("Max. recursion depth=%i\n", depthr);
    }

    if (test) {
        int test_res = test_suffix_array(t, sa, alphabet, n, a);
        printf(test_res ? "Suffix array is correct\n" : "Suffix array is incorrect\n");
        test_res = test_lyndon_array(t, la, alphabet, n, a);                                              
        printf(test_res ? "Lyndon array is correct\n" : "Lyndon array is incorrect\n");
    }

    printf("Working space: %zu bytes + %zu bytes (bkt-array) + %zu bytes (alphabet)\n", max_memory, max_bkt_memory, max_alphabet_memory);
    printf("Working space per input byte: %zu \n", (max_memory + max_bkt_memory + max_alphabet_memory) / (size_t) n);

    // Free memory
    free(bkt);
    free(sa);

    sa = NULL;
    bkt = NULL;
    return la;
}     



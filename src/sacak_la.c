#include "sacak_la.h"

int debug = 0;

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
            printf("  - ");
        } else {
            printf(" %3u", sa[i]);
        }
        if (i < n-1) {
            printf(",");
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
            printf("  - ");
        } else {
            printf(" %3u", la[i]);
        }
        if (i < n-1) {
            printf(",");
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
            printf("  - ");
        } else {
            printf(" %3u", next[i]);
        }
        if (i < n-1) {
            printf(",");
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
*  Computes the bucket array for an string t
*  If head==1, stores pointers to the head of the buckets, otherwise to the tails
*  If rec==1, takes the entries of t as their lex. values
*/ 
void get_bkt_array(unsigned int* t, unsigned int* bkt, unsigned char* alphabet, size_t n, size_t a, int head, int rec) {
    for (int i = 0; i < a; i++) {
        bkt[i] = 0;
    }
    for (int i = 0; i < n; i++) {
        if (rec) {
            bkt[t[i]] += 1; 
        } else {
            bkt[alphabet[(int) t[i]]] += 1;                                                         // alphabet[(int) t[i]] is the value of the symbol at i,
        }
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
int sacak_rec(unsigned int* t, unsigned int* sa, unsigned int* bkt, unsigned char* alphabet, 
                size_t n, size_t a, int depth, size_t t1_len) {
        // Induced sorting of the LMS-type suffixes, algorithm published by Nong et al. in 
        // "Two efficient algorithms for linear time suffixarray construction" (access with IEEE)

        // In the steps, the constant UINT_MAX will be used to represent -1 which is equivalent
        // for int->uint: -1 = -1 mod UINT_MAX = UINT_MAX
        
        if (depth > 0) {
            if (debug) { 
            printf("\nInduced sorting of LMS-substrings, depth=%u\n", depth); 
            printf("0 is of type S");                                                               // always correct
            }

            for(int i = 0; i < n; i++) {                                                                // initialize sa with zeroes
                sa[i] = UINT_MAX;
            }
            get_bkt_array(t, bkt, alphabet, n, a, 0, 1);                                            // get pointers to the end of the buckets
            int cur_type = 0;                                                                           
            int last_type = 1;                                                                      // t[n] = $ is S-type
            sa[bkt[(int) t[n-1] ]] = n-1;                                                           // no need to shift bkt pointer
            for (int i = n-2; i >= 0; i--) {                                                        // left to right scan of t                                                    
                cur_type = lex_compare_symbols(t[i], t[i+1]) == 0 ?                                     
                            (last_type ? 1 : 0) :                                                   // t[i] == t[i+1] -> depends on the type of t[i+1]
                            (lex_compare_symbols(t[i], t[i+1]) == 1 ? 0 : 1);                       // t[i] != t[i+1] -> direct comparison     

                if (cur_type == 0 && last_type == 1) {                                              // insert LMS-suffixes into their buckets in sa
                    if (debug) {
                        printf("*");
                    }
                    sa[bkt[(int) t[i+1]]] = i+1;                                                    // alphabet[(int) t[i]] is the value if the symbol at t[i]
                    bkt[(int) t[i+1]]--;                                                            // shift bucket tail pointer one position to the left
                }
                if (debug) {
                    printf("\n%u is of type %c", t[i], cur_type ? 'S' : 'L');
                }
                last_type = cur_type;
            }
            if (debug) {
                printf("\n\n");   
                print_suffix_array(sa, n, -1);    
            }

            // step 1.2: induce L-type LMS-prefixes
            get_bkt_array(t, bkt, alphabet, n, a, 1, 1);
            for (int i = 0; i < n; i++) {                                                               // left-to-right scan of t
                if (sa[i] != UINT_MAX && sa[i] > 0 && lex_compare_symbols(t[sa[i]-1], t[sa[i]]) > -1) { // t[sa[i]-1] is L-type if t[sa[i]-1] >= t[sa[i]] 
                                                                                                        // since only the L-type block left of an LMS-type symbol is considered here
                    sa[bkt[(int) t[sa[i]-1] ]] = sa[i] - 1;                                   // insert the suffix t[sa[i]-1] into the head of its bucket
                    bkt[(int) t[sa[i]-1 ]]++;                                                 // shift bucket head pointer one position to the right
                    if (i > 0) {sa[i] = 0;}                                                             // dont delete sa[0] as it is LMS-type
                    if (debug) {
                        print_suffix_array(sa, n, i);
                    }
                }
            }

            // step 1.3: induce S-type-prefixes
            get_bkt_array(t, bkt, alphabet, n, a, 0, 1);
            for (int i = n-1; i > 0; i--) {                                                             // right-to-left scan of t
                if (sa[i] != UINT_MAX && sa[i] > 0 && lex_compare_symbols(t[sa[i]-1], t[sa[i]]) < 1) {  // t[sa[i]-1] is s-type if t[sa[i]-1] <= t[sa[i]] 
                                                                                                        // since only the S-type block right of an LMS-type symbol is considered here
                    sa[bkt[(int) t[sa[i]-1] ]] = sa[i] - 1;                                   // insert the suffix t[sa[i]-1] into the tail of its bucket
                    bkt[(int) t[sa[i]-1 ]]--;                                                 // shift bucket tail pointer one position to the left
                    sa[i] = 0;
                    if (debug) {
                        print_suffix_array(sa, n, i);
                    }
                }
            }

            // at this point SA[1..n] contains all sorted LMS-substrings (careful not LMS-suffixes)
            // step 2: rename every LMS-substring as an Integer and store the counter

            // step 2.1: put the LMS-substrings into SA[0..n1-1]
            int n1 = 0;                                                                                 // n1 := |t1| -> #of LMs-substrngs in t
            for (int i = 0; i < n; i++) {
                if (sa[i] != 0) {
                    int temp = sa[i];
                    sa[i] = 0;
                    sa[n1] = temp;
                    n1++;
                }
            }

            if (debug) {
                printf("\nRenaming LMS-substrings\n");
            }

            // step 2.2: compare the LMS-substrings, if they are different, the smaller ones get a lower value
            int last_lms_pos = 0;                                                                       // int t not sa since that value will be  overidden !!!
            int last_lms_len = 0;                                                                       
            int name_offset = 0;        
            int name_counter = 0;

            // create buckets in SA[0..n1-1] which store the number of appearances of an LMS-substring
            for (int i = 0; i < n1; i++) {
                int cur_lms_pos = sa[i];                                                                    // for clarity
                int cur_lms_len = 0;

                // get the length of the LMS-substring sa[i]
                if (sa[i] == n-1) {                                                                     // LMS-substring $ has length 1
                    cur_lms_len = 1;
                } else {
                    int j = 1;
                    while(lex_compare_symbols(t[sa[i] + j - 1], t[sa[i] + j]) < 1) {                    // S-type-block to the right of the LMS-symbol
                        cur_lms_len++;
                        j++;
                    }
                    if (sa[i] + j == n-1) {                                                             // reached the termination symbol
                        cur_lms_len++;
                    } else {
                        while(sa[i] + j < n && 
                                lex_compare_symbols(t[sa[i] + j -1], t[sa[i] + j]) > -1) {              // L-type-block to the left of the next LMS-symbol
                            cur_lms_len++;
                            j++;
                        }
                        if (sa[i]+j == n) {                                                             // add the termination symbol to the length if necessary
                            cur_lms_len++;
                        }
                    }
                }

                int is_different = 0;
                if (last_lms_len != cur_lms_len) {                                                      // both LMS-substrings are for sure different
                    is_different = 1;
                } else {                                                                                // compare which one is lexicographically larger
                    for (int j = 0; j < cur_lms_len; j++) {
                        if (cur_lms_pos + j > n-1 || last_lms_pos + j > n-1) {                  // if one of the LMS-substrings ends with $ the first one has lower value
                            is_different = 1;
                            break;
                        }
                        if (t[cur_lms_pos + j] != t[last_lms_pos + j]) {                                // if the LMS-substrings differ in a position the first has lower value
                            is_different = 1;
                            break;
                        }
                    }
                }

                last_lms_pos = cur_lms_pos;                                                             
                last_lms_len = cur_lms_len;
                if (is_different) {                                                                     // create a new name
                    name_counter++;                                                                     // store the current position so it can be incremented if necessary
                        
                }
                sa[n1 + name_offset] = name_counter - 1;                                                // store the names in SA[n1..n-1], fits since n1 < n/2
                name_offset++;       
                if (debug) {
                    print_suffix_array(sa, n, n1 + name_offset - 1);
                }                                                                   
            }
            
            // save the reduced string backwards into sa[n-n1-1..n] (such that the smallest symbol is the rightmost)
            for (int i = 0; i < n1; i++) {
                sa[n - i - 1] = sa[n1 + i];    
                if (debug) {
                    print_suffix_array(sa, n, n - n1 + i);
                } 
            }

            // step 2.3: test if every character in t1 is unique
            int is_unique = 1;
            for (int i = 1; i < n1; i++) {
                if (sa[n - i] == sa[n - i -1]) {
                    is_unique = 0;
                }
            }

            unsigned int* t1 = sa + n - n1;
            unsigned int* sa1 = sa;
            
            if (is_unique) {
                // TODO: Get sa1 directly from t1 
                for (int i = 0; i < n1; i++) {                                                          
                    sa1[t1[i]] = i;
                }
                if (debug) {
                    print_suffix_array(sa, n, n1);
                }
            } else {
                // TODO: Get sa1 by recursion with sa1 := sa[0..n-n1-1], t1 := sa[n-n1-1..n-1]

                sacak_rec(t1, sa1, bkt, alphabet, n - n1, name_counter, depth + 1, n1);                             // n-n1 =: length of sa1, n1 =: length of t1
            }
            // suffix array of sa1 is now in sa[0..n1]

            // step 3: induce sa from sa1
            // find pointers to the LMS-substrings in t1 and store them in s1[0..n1]
            get_bkt_array(t, bkt, alphabet, n, a, 0, 1);                                                   // get pointers to the end of the buckets 
            int j = n1;
            cur_type = 0;                                                                           
            last_type = 1;                                                                              // t[n] = $ is S-type
            for (int i = n-2; i >= 0; i--) {                                                            // left to right scan of t                                                    
                cur_type = lex_compare_symbols(t[i], t[i+1]) == 0 ?                                     
                            (last_type ? 1 : 0) :                                                       // t[i] == t[i+1] -> depends on the type of t[i+1]
                            (lex_compare_symbols(t[i], t[i+1]) == 1 ? 0 : 1);                           // t[i] != t[i+1] -> direct comparison     

                if (cur_type == 0 && last_type == 1) {                                                  // insert LMS-suffixes into their buckets in sa
                    j--;
                    t1[j] = i+1;
                }
                last_type = cur_type;
            }
            if (debug) {
                print_suffix_array(sa, n, n - n1 + j);
            }
            // order the lms-suffixes using sa1
            for (int i = 0; i < n1; i++) {
                sa1[i] = t1[sa1[i]];
            }
            if (debug) {
                print_suffix_array(sa, n, n1);
            }
            // clean rest of sa
            for (int i = n1; i < n; i++) {
                sa[i] = UINT_MAX;
            }
            if (debug) {
                print_suffix_array(sa, n, n1);
            }

            // put the ordered LMS-substrings into the ends of their bucket
            print_bkt_array(bkt, a, -1);
            for (int i = n1-1; i >= 0; i--) {
                j = sa[i];
                sa[i] = UINT_MAX;
                sa[bkt[(int) t[j] ]] = j;
                bkt[(int) t[j] ]--;
            }
            if (debug) {
                print_suffix_array(sa, n, n1);
            }

            // step 3.2: Induce L-type suffixes from the ordered LMS-type-suffixes, for comments look step 1.2
            get_bkt_array(t, bkt, alphabet, n, a, 1, 1);
            for (int i = 0; i < n; i++) {                                                               
                if (sa[i] != UINT_MAX && sa[i] > 0 && lex_compare_symbols(t[sa[i]-1], t[sa[i]]) > -1) {
                    sa[bkt[(int) t[sa[i]-1] ]] = sa[i] - 1;
                    bkt[(int) t[sa[i]-1 ]]++;
                    if (debug) {
                        print_suffix_array(sa, n, i);
                    }
                }
            }

            // step 3.3: Induce S-type suffixes do this in the calling method since LA is induced in this step
            get_bkt_array(t, bkt, alphabet, n, a, 0, 1);
            for (int i = n-1; i > 0; i--) {                                                         // right-to-left scan of t
                if (sa[i] != UINT_MAX && sa[i] > 0 && lex_compare_symbols(t[sa[i]-1], t[sa[i]]) < 1) {  // t[sa[i]-1] is s-type if t[sa[i]-1] <= t[sa[i]] 
                                                                                                    // since only the S-type block right of an LMS-type symbol is considered here
                    sa[bkt[(int) t[sa[i]-1] ]] = sa[i] - 1;                                         // insert the suffix t[sa[i]-1] into the tail of its bucket
                    bkt[(int) t[sa[i]-1 ]]--;                                                       // shift bucket tail pointer one position to the left
                    if (debug) {
                        print_suffix_array(sa, n, i);
                    }
                }
            }

            return 0;
        } 

        // top-level call

        // step 1: induced sort lms-type suffixes
        // step 1.1: find all LMS-suffixes
        
        if (debug) { 
            printf("\nInduced sorting of LMS-substrings, depth=0\n"); 
            printf("$ is of type S");                                                               // always correct
        }

        for(int i = 0; i < n; i++) {                                                                // initialize sa with zeroes
            sa[i] = UINT_MAX;
        }
        get_bkt_array(t, bkt, alphabet, n, a, 0, 0);                                                   // get pointers to the end of the buckets 
        int cur_type = 0;                                                                           
        int last_type = 1;                                                                          // t[n] = $ is S-type
        sa[bkt[alphabet[(int) t[n-1]]]] = n-1;                                                      // no need to shift bkt pointer
        for (int i = n-2; i >= 0; i--) {                                                            // left to right scan of t                                                    
            cur_type = lex_compare_symbols(t[i], t[i+1]) == 0 ?                                     
                        (last_type ? 1 : 0) :                                                       // t[i] == t[i+1] -> depends on the type of t[i+1]
                        (lex_compare_symbols(t[i], t[i+1]) == 1 ? 0 : 1);                           // t[i] != t[i+1] -> direct comparison     

            if (cur_type == 0 && last_type == 1) {                                                  // insert LMS-suffixes into their buckets in sa
                if (debug) {
                    printf("*");
                }
                sa[bkt[alphabet[(int) t[i+1]]]] = i+1;                                              // alphabet[(int) t[i]] is the value if the symbol at t[i]
                bkt[alphabet[(int) t[i+1]]]--;                                                      // shift bucket tail pointer one position to the left
            }
            if (debug) {
                printf("\n%c is of type %c", t[i], cur_type ? 'S' : 'L');
            }
            last_type = cur_type;
        }
        if (debug) {
            printf("\n\n");       
        }

        // step 1.2: induce L-type LMS-prefixes
        get_bkt_array(t, bkt, alphabet, n, a, 1, 0);
        for (int i = 0; i < n; i++) {                                                               // left-to-right scan of t
            if (sa[i] != UINT_MAX && sa[i] > 0 && lex_compare_symbols(t[sa[i]-1], t[sa[i]]) > -1) { // t[sa[i]-1] is L-type if t[sa[i]-1] >= t[sa[i]] 
                                                                                                    // since only the L-type block left of an LMS-type symbol is considered here
                sa[bkt[alphabet[(int) t[sa[i]-1] ]]] = sa[i] - 1;                                   // insert the suffix t[sa[i]-1] into the head of its bucket
                bkt[alphabet[(int) t[sa[i]-1 ]]]++;                                                 // shift bucket head pointer one position to the right
                if (i > 0) {sa[i] = 0;}                                                             // dont delete sa[0] as it is LMS-type
                if (debug) {
                    print_suffix_array(sa, n, i);
                }
            }
        }

        // step 1.3: induce S-type-prefixes
        get_bkt_array(t, bkt, alphabet, n, a, 0, 0);
        for (int i = n-1; i > 0; i--) {                                                             // right-to-left scan of t
            if (sa[i] != UINT_MAX && sa[i] > 0 && lex_compare_symbols(t[sa[i]-1], t[sa[i]]) < 1) {  // t[sa[i]-1] is s-type if t[sa[i]-1] <= t[sa[i]] 
                                                                                                    // since only the S-type block right of an LMS-type symbol is considered here
                sa[bkt[alphabet[(int) t[sa[i]-1] ]]] = sa[i] - 1;                                   // insert the suffix t[sa[i]-1] into the tail of its bucket
                bkt[alphabet[(int) t[sa[i]-1 ]]]--;                                                 // shift bucket tail pointer one position to the left
                sa[i] = 0;
                if (debug) {
                    print_suffix_array(sa, n, i);
                }
            }
        }

        // at this point SA[1..n] contains all sorted LMS-substrings (careful not LMS-suffixes)
        // step 2: rename every LMS-substring as an Integer and store the counter

        // step 2.1: put the LMS-substrings into SA[0..n1-1]
        int n1 = 0;                                                                                 // n1 := |t1| -> #of LMs-substrngs in t
        for (int i = 0; i < n; i++) {
            if (sa[i] != 0) {
                int temp = sa[i];
                sa[i] = 0;
                sa[n1] = temp;
                n1++;
            }
        }

        if (debug) {
            printf("\nRenaming LMS-substrings\n");
        }

        // step 2.2: compare the LMS-substrings, if they are different, the smaller ones get a lower value
        int last_lms_pos = 0;                                                                       // int t not sa since that value will be  overidden !!!
        int last_lms_len = 0;                                                                       
        int name_offset = 0;        
        int name_counter = 0;

        // create buckets in SA[0..n1-1] which store the number of appearances of an LMS-substring
        for (int i = 0; i < n1; i++) {
            int cur_lms_pos = sa[i];                                                                    // for clarity
            int cur_lms_len = 0;

            // get the length of the LMS-substring sa[i]
            if (sa[i] == n-1) {                                                                     // LMS-substring $ has length 1
                cur_lms_len = 1;
            } else {
                int j = 1;
                while(lex_compare_symbols(t[sa[i] + j - 1], t[sa[i] + j]) < 1) {                    // S-type-block to the right of the LMS-symbol
                    cur_lms_len++;
                    j++;
                }
                if (sa[i] + j == n-1) {                                                             // reached the termination symbol
                    cur_lms_len++;
                } else {
                    while(sa[i] + j < n && 
                            lex_compare_symbols(t[sa[i] + j -1], t[sa[i] + j]) > -1) {              // L-type-block to the left of the next LMS-symbol
                        cur_lms_len++;
                        j++;
                    }
                    if (sa[i]+j == n) {                                                             // add the termination symbol to the length if necessary
                        cur_lms_len++;
                    }
                }
            }

            int is_different = 0;
            if (last_lms_len != cur_lms_len) {                                                      // both LMS-substrings are for sure different
                is_different = 1;
            } else {                                                                                // compare which one is lexicographically larger
                for (int j = 0; j < cur_lms_len; j++) {
                    if (cur_lms_pos + j > n-1 || last_lms_pos + j > n-1) {                  // if one of the LMS-substrings ends with $ the first one has lower value
                        is_different = 1;
                        break;
                    }
                    if (t[cur_lms_pos + j] != t[last_lms_pos + j]) {                                // if the LMS-substrings differ in a position the first has lower value
                        is_different = 1;
                        break;
                    }
                }
            }

            last_lms_pos = cur_lms_pos;                                                             
            last_lms_len = cur_lms_len;
            if (is_different) {                                                                     // create a new name
                name_counter++;                                                                     // store the current position so it can be incremented if necessary
                    
            }
            sa[n1 + name_offset] = name_counter - 1;                                                // store the names in SA[n1..n-1], fits since n1 < n/2
            name_offset++;       
            if (debug) {
                print_suffix_array(sa, n, n1 + name_offset - 1);
            }                                                                   
        }
        
        // save the reduced string backwards into sa[n-n1-1..n] (such that the smallest symbol is the rightmost)
        for (int i = 0; i < n1; i++) {
            sa[n - i - 1] = sa[n1 + i];    
            if (debug) {
                print_suffix_array(sa, n, n - n1 + i);
            } 
        }

        // step 2.3: test if every character in t1 is unique
        int is_unique = 1;
        for (int i = 1; i < n1; i++) {
            if (sa[n - i] == sa[n - i -1]) {
                is_unique = 0;
            }
        }

        unsigned int* t1 = sa + n - n1;
        unsigned int* sa1 = sa;
        
        if (is_unique) {
            // TODO: Get sa1 directly from t1 
            for (int i = 0; i < n1; i++) {                                                          
                sa1[t1[i]] = i;
            }
            if (debug) {
                print_suffix_array(sa, n, n1);
            }
        } else {
            // TODO: Get sa1 by recursion with sa1 := sa[0..n-n1-1], t1 := sa[n-n1-1..n-1]
            int temp = ascii;
            ascii = 1;                                                                              // dont use the alphabet conversion for this part
            sacak_rec(t1, sa1, bkt, alphabet, n1, name_counter, depth + 1, n1);                 // n1 =: length of sa1, n1 =: length of t1
            ascii = temp;                                                                           // enable it again
        }
        // suffix array of sa1 is now in sa[0..n1]

        // step 3: induce sa from sa1
        // find pointers to the LMS-substrings in t1 and store them in s1[0..n1]
        get_bkt_array(t, bkt, alphabet, n, a, 0, 0);                                                   // get pointers to the end of the buckets 
        int j = n1;
        cur_type = 0;                                                                           
        last_type = 1;                                                                              // t[n] = $ is S-type
        for (int i = n-2; i >= 0; i--) {                                                            // left to right scan of t                                                    
            cur_type = lex_compare_symbols(t[i], t[i+1]) == 0 ?                                     
                        (last_type ? 1 : 0) :                                                       // t[i] == t[i+1] -> depends on the type of t[i+1]
                        (lex_compare_symbols(t[i], t[i+1]) == 1 ? 0 : 1);                           // t[i] != t[i+1] -> direct comparison     

            if (cur_type == 0 && last_type == 1) {                                                  // insert LMS-suffixes into their buckets in sa
                j--;
                t1[j] = i+1;
            }
            last_type = cur_type;
        }
        if (debug) {
            print_suffix_array(sa, n, n - n1 + j);
        }
        // order the lms-suffixes using sa1
        for (int i = 0; i < n1; i++) {
            sa1[i] = t1[sa1[i]];
        }
        if (debug) {
            print_suffix_array(sa, n, n1);
        }
        // clean rest of sa
        for (int i = n1; i < n; i++) {
            sa[i] = UINT_MAX;
        }
        if (debug) {
            print_suffix_array(sa, n, n1);
        }

        // put the ordered LMS-substrings into the ends of their bucket
        print_bkt_array(bkt, a, -1);
        for (int i = n1-1; i >= 0; i--) {
            j = sa[i];
            sa[i] = UINT_MAX;
            sa[bkt[alphabet[(int) t[j] ]]] = j;
            bkt[alphabet[(int) t[j] ]]--;
        }
        if (debug) {
            print_suffix_array(sa, n, n1);
        }

        // step 3.2: Induce L-type suffixes from the ordered LMS-type-suffixes, for comments look step 1.2
        get_bkt_array(t, bkt, alphabet, n, a, 1, 0);
        for (int i = 0; i < n; i++) {                                                               
            if (sa[i] != UINT_MAX && sa[i] > 0 && lex_compare_symbols(t[sa[i]-1], t[sa[i]]) > -1) {
                sa[bkt[alphabet[(int) t[sa[i]-1] ]]] = sa[i] - 1;
                bkt[alphabet[(int) t[sa[i]-1 ]]]++;
                if (debug) {
                    print_suffix_array(sa, n, i);
                }
            }
        }

        // step 3.3: Induce S-type suffixes do this in the calling method since LA is induced in this step
        get_bkt_array(t, bkt, alphabet, n, a, 0, 0);
        for (int i = n-1; i > 0; i--) {                                                            // right-to-left scan of t
            if (sa[i] != UINT_MAX && sa[i] > 0 && lex_compare_symbols(t[sa[i]-1], t[sa[i]]) < 1) {  // t[sa[i]-1] is s-type if t[sa[i]-1] <= t[sa[i]] 
                                                                                                    // since only the S-type block right of an LMS-type symbol is considered here
                sa[bkt[alphabet[(int) t[sa[i]-1] ]]] = sa[i] - 1;                                   // insert the suffix t[sa[i]-1] into the tail of its bucket
                bkt[alphabet[(int) t[sa[i]-1 ]]]--;                                                 // shift bucket tail pointer one position to the left
                if (debug) {
                    print_suffix_array(sa, n, i);
                }
            }
        }
       
        return 0;
}



unsigned int* sacak_la_two_aux(unsigned int* t, unsigned char* alphabet, size_t n, size_t a) {
        
        // Memory allocation
        unsigned int* sa = (unsigned int*) malloc(n * sizeof(unsigned int));
        unsigned int* la = (unsigned int*) malloc(n * sizeof(unsigned int));
        unsigned int* next = (unsigned int*) malloc(n * sizeof(unsigned int));                
        unsigned int* prev = (unsigned int*) malloc(n * sizeof(unsigned int));

        unsigned int* bkt = (unsigned int*) malloc(a * sizeof(unsigned int));                       // bkt has size sigma + 1, sigma < 255
        
        if (sa == 0 || la == 0 || next == 0 || prev == 0 || bkt == 0) {
            perror("malloc: ");
            printf("Could not allocate memory for la, sa, next, prev\n");
            exit(-1);
        }

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


unsigned int* sacak_la_one_aux(unsigned int* t, unsigned char* alphabet, size_t n, size_t a) {
    return 0;
}     
unsigned int* sacak_la_inplace(unsigned int* t, unsigned char* alphabet, size_t n, size_t a) {
    return 0;
}     



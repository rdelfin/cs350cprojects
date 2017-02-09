#include "mmult.h"

#include "clock.h"

#include <stdio.h>
#include <stdlib.h>

static inline char chop(int val) {
   return val < -128 ? -128 : (val > 127 ? 127 : val);
}

void multa(int n, char* A, char* B, char* C) {
    int j, i, k;
    for(i = 0; i < n; i++) {
        for(j = 0; j < n; j++) {
            int sum = 0;
            for(k = 0; k < n; k++) {
                sum += A[i*n + k]*B[k*n + j];
            }
            C[i*n + j] = chop(C[i*n + j] + sum);
        }
    }
}

void multb(int n, char* A, char* B, char* C) {
    int j, i, k;
    for(j = 0; j < n; j++) {
        for(i = 0; i < n; i++) {
            int sum = 0;
            for(k = 0; k < n; k++) {
                sum += A[i*n + k]*B[k*n + j];
            }
            C[i*n + j] = chop(C[i*n + j] + sum);
        }
    }
}

void multc(int n, char* A, char* B, char* C) {
    int j, i, k;
    for(j = 0; j < n; j++) {
        for(k = 0; k < n; k++) {
            int r = B[k*n + j];
            for(i = 0; i < n; i++) {
                C[i*n + j] = chop(C[i*n + j] + A[i*n + k]*r);
            }
        }
    }
}

void multd(int n, char* A, char* B, char* C) {
    int j, i, k;
    for(k = 0; k < n; k++) {
        for(j = 0; j < n; j++) {
            int r = B[k*n + j];
            for(i = 0; i < n; i++) {
                C[i*n + j] = chop(C[i*n + j] + A[i*n + k]*r);
            }
        }
    }
}

void multe(int n, char* A, char* B, char* C) {
    int j, i, k;
    for(k = 0; k < n; k++) {
        for(i = 0; i < n; i++) {
            int r = A[i*n + k];
            for(j = 0; j < n; j++) {
                C[i*n + j] = chop(C[i*n + j] + r*B[k*n + j]);
            }
        }
    }
}

void multf(int n, char* A, char* B, char* C) {
    int j, i, k;
    for(i = 0; i < n; i++) {
        for(k = 0; k < n; k++) {
            int r = A[i*n + k];
            for(j = 0; j < n; j++) {
                C[i*n + j] = chop(C[i*n + j] + r*B[k*n + j]);
            }
        }
    }
}


double multa_timed(int n, char* A, char* B, char* C) {
    double tot_time = 0;
    int count = 0;
    int j, i, k;
    for(i = 0; i < n; i++) {
        for(j = 0; j < n; j++) {
            start_counter();

            int sum = 0;
            for(k = 0; k < n; k++) {
                sum += A[i*n + k]*B[k*n + j];
            }
            C[i*n + j] = chop(C[i*n + j] + sum);
            
            tot_time += get_counter();
            count++;
        }
    }

    return tot_time / count;
}


double multb_timed(int n, char* A, char* B, char* C) {
    double tot_time = 0;
    int count = 0;
    int j, i, k;
    for(j = 0; j < n; j++) {
        for(i = 0; i < n; i++) {
            start_counter();

            int sum = 0;
            for(k = 0; k < n; k++) {
                sum += A[i*n + k]*B[k*n + j];
            }
            C[i*n + j] = chop(C[i*n + j] + sum);
            
            tot_time += get_counter();
            count++;
        }
    }

    return tot_time / count;
}

double multc_timed(int n, char* A, char* B, char* C) {
    int count = 0;
    double tot_time = 0;
    int j, i, k;
    for(j = 0; j < n; j++) {
        for(k = 0; k < n; k++) {
            start_counter();

            int r = B[k*n + j];
            for(i = 0; i < n; i++) {
                C[i*n + j] = chop(C[i*n + j] + A[i*n + k]*r);
            }
            
            tot_time += get_counter();
            count++;
        }
    }
    
    return tot_time / count;
}

double multd_timed(int n, char* A, char* B, char* C) {
    double tot_time = 0;
    int count = 0;
    int j, i, k;
    for(k = 0; k < n; k++) {
        for(j = 0; j < n; j++) {
            start_counter();

            int r = B[k*n + j];
            for(i = 0; i < n; i++) {
                C[i*n + j] = chop(C[i*n + j] + A[i*n + k]*r);
            }
            
            tot_time += get_counter();
            count++;
        }
    }
    
    return tot_time / count;
}

double multe_timed(int n, char* A, char* B, char* C) {
    double tot_time = 0;
    int count = 0;
    int j, i, k;
    for(k = 0; k < n; k++) {
        for(i = 0; i < n; i++) {
            start_counter();

            int r = A[i*n + k];
            for(j = 0; j < n; j++) {
                C[i*n + j] = chop(C[i*n + j] + r*B[k*n + j]);
            }
            
            tot_time += get_counter();
            count++;
        }
    }
    
    return tot_time / count;
}

double multf_timed(int n, char* A, char* B, char* C) {
    double tot_time = 0;
    int count = 0;
    int j, i, k;
    for(i = 0; i < n; i++) {
        for(k = 0; k < n; k++) {
            start_counter();

            int r = A[i*n + k];
            for(j = 0; j < n; j++) {
                C[i*n + j] = chop(C[i*n + j] + r*B[k*n + j]);
            }
            
            tot_time += get_counter();
            count++;
        }
    }
    
    return tot_time / count;
}

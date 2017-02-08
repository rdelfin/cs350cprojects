#include "mmult.h"

static inline char chop(int val) {
   return val < -128 ? -128 : (val > 127 ? 127 : val);
}

void multa(int n, char A[n][n], char B[n][n], char C[n][n]) {
    int j, i, k;
    for(i = 0; i < n; i++) {
        for(j = 0; j < n; j++) {
            int sum = 0;
            for(k = 0; k < n; k++) {
                sum += A[i][k]*B[k][j];
            }
            C[i][j] = chop(C[i][j] + sum);
        }
    }
}

void multb(int n, char A[n][n], char B[n][n], char C[n][n]) {
    int j, i, k;
    for(j = 0; j < n; j++) {
        for(i = 0; i < n; i++) {
            int sum = 0;
            for(k = 0; k < n; k++) {
                sum += A[i][k]*B[k][j];
            }
            C[i][j] = chop(C[i][j] + sum);
        }
    }
}

void multc(int n, char A[n][n], char B[n][n], char C[n][n]) {
    int j, i, k;
    for(j = 0; j < n; j++) {
        for(k = 0; k < n; k++) {
            int r = B[k][j];
            for(i = 0; i < n; i++) {
                C[i][j] = chop(C[i][j] + A[i][k]*r);
            }
        }
    }
}

void multd(int n, char A[n][n], char B[n][n], char C[n][n]) {
    int j, i, k;
    for(k = 0; k < n; k++) {
        for(j = 0; j < n; j++) {
            int r = B[k][j];
            for(i = 0; i < n; i++) {
                C[i][j] = chop(C[i][j] + A[i][k]*r);
            }
        }
    }
}

void multe(int n, char A[n][n], char B[n][n], char C[n][n]) {
    int j, i, k;
    for(k = 0; k < n; k++) {
        for(i = 0; i < n; i++) {
            int r = A[i][k];
            for(j = 0; j < n; j++) {
                C[i][j] = chop(C[i][j] + r*B[k][j]);
            }
        }
    }
}

void multf(int n, char A[n][n], char B[n][n], char C[n][n]) {
    int j, i, k;
    for(i = 0; i < n; i++) {
        for(k = 0; k < n; k++) {
            int r = A[i][k];
            for(j = 0; j < n; j++) {
                C[i][j] = chop(C[i][j] + r*B[k][j]);
            }
        }
    }
}


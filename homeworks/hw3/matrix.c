#include "mmult.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <sys/time.h>


void initialize(char* A, char* B, char* C, int n);
double measure(char* A, char* B, char* C, int n, void (*func)(int, char*, char*, char*));

int main(int argc, char* argv[]) {
    printf("--\t| A\tB\tC\tD\tE\tF\n");

    for(int i = 1; i <= 10; i++) {
        int n = 100*i;

        printf("%d\t| ", n);

        char* A = malloc(sizeof(char) * n*n);
        char* B = malloc(sizeof(char) * n*n);
        char* C = malloc(sizeof(char) * n*n);
        
        initialize(A, B, C, n);
        double timea = measure(A, B, C, n, multa);
        initialize(A, B, C, n);
        double timeb = measure(A, B, C, n, multb);
        initialize(A, B, C, n);
        double timec = measure(A, B, C, n, multc);
        initialize(A, B, C, n);
        double timed = measure(A, B, C, n, multd);
        initialize(A, B, C, n);
        double timee = measure(A, B, C, n, multe);
        initialize(A, B, C, n);
        double timef = measure(A, B, C, n, multf);

        free(A);
        free(B);
        free(C);

        printf("%.4fs\t%.4fs\t%.4fs\t%.4fs\t%.4fs\t%.4fs\t\n", timea, timeb, timec, timed, timee, timef);
    }

    printf("\nCalculating graph...\n====================================\n\n");

    double data2[6][15];

    for(int j = 0; j < 15; j++) {
        int n = 50*(j+1);
        char* A = malloc(sizeof(char) * n*n);
        char* B = malloc(sizeof(char) * n*n);
        char* C = malloc(sizeof(char) * n*n);

        initialize(A, B, C, n);
        data2[0][j] = multa_timed(n, A, B, C);
        initialize(A, B, C, n);
        data2[1][j] = multb_timed(n, A, B, C);
        initialize(A, B, C, n);
        data2[2][j] = multc_timed(n, A, B, C);
        initialize(A, B, C, n);
        data2[3][j] = multd_timed(n, A, B, C);
        initialize(A, B, C, n);
        data2[4][j] = multe_timed(n, A, B, C);
        initialize(A, B, C, n);
        data2[5][j] = multf_timed(n, A, B, C);
    }

    printf("-\t50\t100\t150\t200\t250\t300\t350\t400\t450\t500\t550\t600\t650\t700\t750\n");
    for(int i = 0; i < 6; i++) {
        printf("v%d\t", i);
        for(int j = 0; j < 15; j++) {
            int n = 50*(j+1);
            printf("%.4f\t", data2[i][j]);
        }
        printf("\n");
    }

    return 0;
}

void initialize(char* A, char* B, char* C, int n) {
    int i, j;
    for(i = 0; i < n; i++) {
        for(j = 0; j < n; j++) {
            if(i == j || i + 1 == j || i - 1 == j) {
                A[i*n + j] = 1;
                B[i*n + j] = 1;
            } else {
                A[i*n + j] = 0;
                B[i*n + j] = 0;
            }
            C[i*n + j] = 0;
        }
    }
}

double measure(char* A, char* B, char* C, int n, void (*func)(int, char*, char*, char*)) {
    struct timeval start, end;

    gettimeofday(&start, NULL);
    func(n, A, B, C);
    gettimeofday(&end, NULL);

    double elapsedTime = (end.tv_sec - start.tv_sec);
    elapsedTime += (end.tv_usec - start.tv_usec) / 1000.0 / 1000.0;

    return elapsedTime;
}


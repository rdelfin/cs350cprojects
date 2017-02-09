#include "mmult.h"

#define ROWS (1000)
#define COLUMNS (1000)
#define ENTRIES (ROWS*COLUMNS)

char A[ENTRIES], B[ENTRIES], C[ENTRIES];

int main(int argc, char* argv[]) {
    int i, j;
    for(i = 0; i < ROWS; i++) {
        for(j = 0; j < COLUMNS; j++) {
            if(i == j || i + 1 == j || i - 1 == j) {
                A[i*COLUMNS + j] = 1;
                B[i*COLUMNS + j] = 1;
            } else {
                A[i*COLUMNS + j] = 0;
                B[i*COLUMNS + j] = 0;
            }
            C[i*COLUMNS + j] = 0;
        }
    }

    
}

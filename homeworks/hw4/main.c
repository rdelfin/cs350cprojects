#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "bitmatrix.h"
#include "bitops.h"

bitmatrix_t* read_file(char* fileName, uint8_t diag);
bitmatrix_t* reachability_matrix(bitmatrix_t* mat);

int main(int argc, char* argv[]) {
    if(argc != 2 && argc != 3) {
        if(argc <= 1)
            printf("No arguments provided. Provide a file name and (optionally) a default value for the diagonal\n");
        else
            printf("Too many arguments provided. Provide a file name\n");

        return -1;
    }

    bitmatrix_t* mat = read_file(argv[1], (uint8_t) (argc == 3 ? (atoi(argv[2]) & 1) : 0));

    bitmatrix_t* reachMat = reachability_matrix(mat);

    bitmatrix_print(reachMat);
    
    return 0;
}

/**
 * Read the file format provided for test data, tokenize data and insert into
 * the mat object.
 */
bitmatrix_t* read_file(char* fileName, uint8_t diag) {
    char buffer[128];
    char* line = NULL;
    size_t len;
    FILE* matFile = fopen(fileName, "r");
    
    if(getline(&line, &len, matFile) == -1)
        exit(-1);

    len = atoi(line);
    
    // Will initialize object and set all values in the matrix to 0
    bitmatrix_t* mat = bitmatrix_init(len, len);

    // Iterate over all lines
    while(getline(&line, &len, matFile) != -1) {
        size_t start, end;

        // Tokenize string to get both ints
        strncpy(buffer, line, 128);
        char* ptr = strtok(buffer, " \n\t");
        start = (size_t)atoi(ptr);              // These casts could end in so many tears
        ptr = strtok(NULL, " \n\t");
        end = (size_t)atoi(ptr);

        // Store edge as row: start, col: end
        bitmatrix_set(mat, start, end, 1);
    }

    // Set diagonal to the value of diag
    for(size_t i = 0; diag == 1 && i < len; i++)
        bitmatrix_set(mat, i, i, diag);

    return mat;
}

/*
 * Algorithm used (Warshall's algorithm) can be found outlined in here:
 * http://www.math.uiuc.edu/~ash/Discrete/213Ch3.pdf
 * Essentially, boils down to this. Iterate over every column i. In each
 * iteration, for every row j
 */
bitmatrix_t* reachability_matrix(bitmatrix_t* mat) {
    bitmatrix_t* reachMat = bitmatrix_init_cpy(mat);
    uint8_t rowiBuffer[reachMat->rowBytes];
    uint8_t rowjBuffer[reachMat->rowBytes];

    for(size_t i = 0; i < reachMat->cols; i++) {

        bitmatrix_getrow(reachMat, rowiBuffer, i);

        for(size_t j = 0; j < reachMat->rows; j++) {
            bitmatrix_getrow(reachMat, rowjBuffer, j);
            uint8_t bytemask = lowbitextend_b(bitmatrix_get(mat, j, i));        // Obtain a byte-mask to only change values if bit is 1
            for(size_t k = 0; k < reachMat->rowBytes; k++)                      // Iterate over every byte of the row
                rowjBuffer[k] = rowjBuffer[k] | (bytemask & rowiBuffer[k]);     // OR both rows only if the byte is set to 1

            bitmatrix_setrow(reachMat, rowjBuffer, j);                          // Write data back (regardless of changes)
        }
    }

    return reachMat;
}


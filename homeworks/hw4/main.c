#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "bitmatrix.h"

void read_file(char* fileName, bitmatrix_t*);

int main(int argc, char* argv[]) {
    /**
     * Reads the graph in for
     */
    if(argc != 2) {
        if(argc <= 1)
            printf("No arguments provided. Provide a file name");
        else
            printf("Too many arguments provided. Provide a file name");
    }
    
    bitmatrix_t mat;

    read_file(argv[1], &mat);
    
    return 0;
}

/**
 * Read the file format provided for test data, tokenize data and insert into
 * the mat object.
 */
void read_file(char* fileName, bitmatrix_t* mat) {
    char buffer[128];
    char* line = NULL;
    size_t len;
    FILE* matFile = fopen(fileName, "r");
    
    if(getline(&line, &len, matFile) == -1)
        exit(-1);

    len = atoi(line);
    
    // Will initialize object and set all values in the matrix to 0
    bitmatrix_init(mat, len);
    
    // Iterate over all lines
    while(getline(&line, &len, matFile) != -1) {
        size_t start, end;
        
        // Tokenize string to get both ints
        strncpy(buffer, line, 128);
        char* ptr = strtok(buffer, " \n\t");
        start = atoi(ptr);
        ptr = strtok(NULL, " \n\t");
        end = atoi(ptr);

        // Store edge as row: start, col: end
        bitmatrix_set(&mat, start, end, 1); 
    }
}


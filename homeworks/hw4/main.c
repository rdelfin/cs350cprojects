#include <stdlib.h>
#include <stdio.h>
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

void read_file(char* fileName, bitmatrix_t* mat) {
    char* line = NULL;
    size_t len;
    FILE* matFile = fopen(fileName, "r");
    
    if(getline(&line, &len, matFile) == -1)
        exit(-1);

    len = atoi(line);

    bitmatrix_init(mat, len);

    while(getline(&line, &len, matFile) != -1) {
        printf("LINE: %s\n", line);
    }
}


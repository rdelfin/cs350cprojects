#include "args.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

char* get_content(int argc, char* argv[]) {
    if(argc != 2) {
        fprintf(stderr, "Usage: %s [input-file]\n", argv[0]);
        fprintf(stderr, " The input file provided is the y86 assembly program to be compiled\n");
    }

    return file_to_string(argv[1]);
}

char* file_to_string(char* path) {
    FILE* f = fopen(path, "rb");
    char* string = NULL;
    uint64_t file_length = 0;

    if(f != NULL) {
        fseek(f, 0, SEEK_END);          // Move file cursor to the end of the file
        file_length = ftell(f);         // Get the position of the cursor. Since it's at the end of the file, it will equal the # of bytes in the file
        fseek(f, 0, SEEK_SET);          // Reset cursor to the begining of the file
        string = malloc(file_length);

        // Check if malloc failed (possible since this could be a large file)
        // If not, read into string
        if(string != NULL) {
            fread(string, 1, file_length, f);
        }

        fclose(f);
    }

    return string;
}

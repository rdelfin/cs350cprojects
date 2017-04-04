#include <stdio.h>
#include <unistd.h>

#include "parser.h"



int main(int argc, char* argv[]) {
    if(argc != 2) {
        fprintf(stderr, "Usage: %s [input-file]\n", argv[0]);
        fprintf(stderr, "The input file provided is the y86 binary program to be simulated\n");
        return -1;
    }

    if(access(argv[1], F_OK) == -1) {
        fprintf(stderr, "The file provided does not exist. Try with a different file.");
        return -1;
    }

    state_t* initial_state = parse(argv[1]);
    state_print(initial_state, stdout);

    return 0;
}


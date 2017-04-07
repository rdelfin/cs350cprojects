#include <stdio.h>
#include <unistd.h>

#include "parser.h"
#include "simulator.h"


int main(int argc, char* argv[]) {
    if(argc != 2) {
        fprintf(stderr, "Usage: %s [input-file]\n", argv[0]);
        fprintf(stderr, "The input file provided is the y86 binary program to be simulated\n");
        return -1;
    }

    if(access(argv[1], F_OK) == -1) {
        fprintf(stderr, "The file provided does not exist. Try with a different file.\n");
        return -1;
    }

    state_t* state = parse(argv[1]);
    state_print(state, stdout);

    y86addr_t stat;
    state_get_stat(state, &stat);

    while(stat == STAT_AOK) {
        execute_next_instruction(state);

        state_get_stat(state, &stat);
    }

    printf("\nFinal State:\n==================================\n\n");
    state_print(state, stdout);

    state_destroy(state);

    return 0;
}


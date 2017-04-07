#include <stdio.h>
#include <unistd.h>
#include <time.h>

#include "parser.h"
#include "simulator.h"


void timespec_diff(struct timespec *start, struct timespec *stop,
                   struct timespec *result);
double timespec_to_ms(struct timespec *ts);

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

    // Time keeping
    struct timespec startTime, endTime, diffTime;
    long instr_executed = 0;
    clock_gettime(CLOCK_REALTIME, &startTime);

    y86addr_t stat;
    state_get_stat(state, &stat);

    while(stat == STAT_AOK) {
        instr_executed++;
        execute_next_instruction(state);
        state_get_stat(state, &stat);
    }

    clock_gettime(CLOCK_REALTIME, &endTime);
    timespec_diff(&startTime, &endTime, &diffTime);
    double execution_ms = timespec_to_ms(&diffTime);

    printf("\nFinal State:\n==================================\n\n");
    printf("Execution Time: %.5fms\nInstructions / ms: %.4f\n", execution_ms, instr_executed / execution_ms);
    state_print(state, stdout);

    state_destroy(state);

    return 0;
}

// Shamelessly taken from https://gist.github.com/diabloneo/9619917
void timespec_diff(struct timespec *start, struct timespec *stop,
                   struct timespec *result)
{
    if ((stop->tv_nsec - start->tv_nsec) < 0) {
        result->tv_sec = stop->tv_sec - start->tv_sec - 1;
        result->tv_nsec = stop->tv_nsec - start->tv_nsec + 1000000000;
    } else {
        result->tv_sec = stop->tv_sec - start->tv_sec;
        result->tv_nsec = stop->tv_nsec - start->tv_nsec;
    }

    return;
}

double timespec_to_ms(struct timespec *ts) {
    return (ts->tv_sec*1000.0) + (ts->tv_nsec / 1000000.0);
}

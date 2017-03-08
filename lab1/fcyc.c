
// fcyc.c - Run the test function and estimate the ``best'' runtime
// (in clock cycles).

/*
  This file defines the function fcyc2, which runs a test function
  (supplied in mountain.c) enough times to estimate the best-case
  runtime. If we only ran it once, we'd be more likely to see a
  slowdown due to the initial state of the cache; we run the function
  repeatedly, keeping track of the K best runs as we go (keeping them
  sorted in order of runtime), and when the top K are close enough
  (i.e. the #1/fastest runtime is CLOSE ENOUGH to the #K runtime), we
  have ``converged,'' and the fastest runtime is returned.

  This file contains two lab questions, #2 and #3.
*/

#include <stdlib.h>
#include <sys/times.h>
#include <stdio.h>

#include "clock.h"
#include "fcyc.h"

#define K 3                 // We record the top K runs; higher K
                            // means the algorithm takes longer to
                            // converge, but we are more likely to get
                            // a better estimate

#define MAXSAMPLES 20       // Maximum # of times we run the test
                            // function and record its runtime

#define EPSILON 0.01        // Convergence epsilon (see algorithm)

// We use static variables for each of the above #defines; this allows
// us to use the #defines as defaults and allow the caller to change
// them if (s)he wishes.
static int kbest = K;
static int maxsamples = MAXSAMPLES;
static double epsilon = EPSILON;

static double *values = NULL; // array of sample values, sorted in
                              // increasing order
static int samplecount = 0;   // number of samples in values array
                              // (thus far)

// Allocate space for the values array, which will hold the K best
// cycle counts from the test runs.
static void init_sampler()
{
    if (values) free(values);

    // Allocate values array & init to 0
    values = calloc(kbest, sizeof(double));

    samplecount = 0; /* Initialize samplecount */
}

// Add new sample to sorted list of values. If val > values[kbest],
// the value gets ignored.
static void add_sample(double val)
{
    // CS 350c: Explain what the code in this section is doing by
    // answering the following questions:

    //   (2) What is the role of C-variable pos, and what is its value
    //       set to by the if/else statements?

    //   (3) How does this code keep the array named "values" sorted?
    //       What sorting algorithm is (implicitly) used?

    int pos = 0;
    if (samplecount < kbest) {
        pos = samplecount;
        values[pos] = val;
    } else if (val < values[kbest-1]) {
        pos = kbest-1;
        values[pos] = val;
    }

    samplecount++;
    while (pos > 0 && values[pos-1] > values[pos]) {
        double temp = values[pos-1];
        values[pos-1] = values[pos];
        values[pos] = temp;
        pos--;
    }
}

static int has_converged()

// This function determines whether the kbest measurements have
// converged within epsilon.  We compare the difference between our
// worst (biggest) measurement with our best (smallest) - if the
// relative error is less than epsilon, we have converged.
{
    return
        (samplecount >= kbest) && /* don't converge unless we've taken
                                     at least kbest samples */
        ((1 + epsilon)*values[0] >= values[kbest-1]);
}

double fcyc2(test_funct2 f, unsigned long int *mem )

// Determine the best possible performance we can obtain with test
// function f by taking multiple samples, keeping kbest samples with
// least value, and halting when all the kbest samples are within a
// certain relative error epsilon.

{
    double result;
    init_sampler();

    do {
        double cyc;
        start_counter();
        unsigned long int x = f(mem);
        cyc = get_counter();
        add_sample(cyc);
        fprintf(stderr, "x: %lu\n", x);
    } while (!has_converged() && samplecount < maxsamples);

    result = values[0];
    return result;
}

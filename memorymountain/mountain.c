
// mountain.c - Generate the memory mountain.

/*
   This code originated as some associated material that was made
   available to faculty using Bryant and O'Hallaron's book "Computer
   Systems, A Programmer's Perspective".  Over the years, we modified
   this code so it would work on newer and newer versions of Linux and
   MacOS -- and we removed the code specific to the DEC Alpha.

   On the Internet, one can find many versions of this code.  For our
   class, this set of files are the ones we will use.

   We will use this to explore memory-system performance.  We have
   added additional comments; maybe these will help students to
   understand the code.

   This code is composed of five files:
     this file
     fcyc.h
     fcyc.c
     clock.h
     clock.c

   This file contains 1 lab question, #1.
*/

#include <stdlib.h>
#include <stdio.h>


#include "fcyc.h"  // K-best measurement timing routines
#include "clock.h" // routines to access the cycle counter

#define MINBYTES  (1 << 10)  // Working set size ranges from 1 KB
#define MAXBYTES  (1 << 26)  // ... up to 64 MB
#define MAXSTRIDE 16         // Strides range from 1 to 16
#define MAXELEMS  MAXBYTES/sizeof(int)

#define SLEEPTIME 4 // time to sleep when estimating the clock frequency
#define VERBOSE 0

int data[MAXELEMS]; // the array we'll be traversing

void init_data(int *data, int n);
void test(int elems, int stride);
double run(int size, int stride, double Mhz);

int main()
{
    int size;        /* Working set size (in bytes) */
    int stride;      /* Stride (in array elements) */
    double Mhz;      /* Clock frequency */

    init_data(data, MAXELEMS);          /* Initialize each element in data to 1 */
    Mhz = mhz(VERBOSE, SLEEPTIME);      /* Estimate the clock frequency */

    printf("Clock frequency is approx. %.1f MHz\n", Mhz);
    printf("Memory mountain (MB/sec)\n");

    /* Print result table header. */

    printf("\t");
    for (stride = 1; stride <= MAXSTRIDE; stride++)
	printf("s%d\t", stride);
    printf("\n");

    /* Execute the run function on varying size and stride. size is
    the number of bytes to traverse, and stride is how many elements
    to ``skip'' after each access. */

    for (size = MAXBYTES; size >= MINBYTES; size >>= 1) {

	if (size > (1 << 20))
	    printf("%dm\t", size / (1 << 20));
	else
	    printf("%dk\t", size / 1024);

	for (stride = 1; stride <= MAXSTRIDE; stride++) {
	    printf("%.1f\t", run(size, stride, Mhz));
	}
	printf("\n");
    }
    exit(0);
}

void init_data(int *data, int n)

/* init_data - initializes the array */
{
    int i;

    for (i = 0; i < n; i++)
	data[i] = 1;
}

void test(int elems, int stride) /* The test function */

// Reads elems elements from the data array, with a particular stride,
// the number of elements to "jump" after each access.
{
    int i, result = 0;

    // We want to prevent the C compiler from realizing that the
    // values we are reading aren't actually being used for anything;
    // to do this, we add up all the values that we read and store
    // them in a volatile variable.  Marking a variable as
    // ``volatile'' essentially says to the compiler: ``don't mess
    // with this variable or try to optimize what I do with it, even
    // if you notice some ineffeciency.''  Since the implementation of
    // the volatile keyword varies by C implementation, we still need
    // to inspect the assembly code produced by the compiler to make
    // sure the elements are being accessed in the way we expect them
    // to be.

    volatile int sink; // Assigning the summation of the values we
                       // read to this variable helps prevent the
                       // compiler from ``skipping'' all these
                       // meaningless reads.

    for (i = 0; i < elems; i += stride) // skip stride elements in the
      result += data[i];                // array each time

    sink = result; // So compiler doesn't optimize away the loop.
}

double run(int size, int stride, double Mhz)

// Run test(elems, stride) and return read throughput (MB/s)
{
    double cycles;
    int elems = size / sizeof(int);

    // These are the parameters that the ``test'' function will be
    // called with, inside the fcyc2 function.

    test(elems, stride); // warms up the cache

    // fcyc2 runs the test function several times, and determines the
    // ``best'' runtime up to a certain degree of error. See the body
    //  of fcyc2 (in fcyc.c) for the details.

    cycles = fcyc2(test, elems, stride); // call test(elems,stride)

    // What's going on here?  We are passing the FUNCTION test as an
    // argument to another function, fcyc2. fcyc2 (defined in fcyc.c)
    // takes a function as an argument, as well as some parameters to
    // be passed to that function and calls it a few times with those
    // parameters. Then, it records the best running time of the
    // function (i.e. the smallest number of cycles needed to run the
    // function) and returns it.

    // For more information on functions as arguments to other
    // functions, see Section 5.11 of Kernighan & Ritchie.

    // We wish to return the read throughput; that is, we are trying
    // to calculate the number of MBytes read per second.

    // CS350c (1): Explain how the next line computes the
    // throughput. Specifically, give the units of each of the
    // variables (size, stride, cycles, Mhz) and explain how the
    // resulting expression gives a result in MB/s.

    return (size / stride) / (cycles / Mhz); // convert cycles to MB/s
}

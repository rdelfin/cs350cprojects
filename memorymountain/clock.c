
// clock.c - Estimate the clock rate of the system

/*
  This file defines the function mhz, which approximates the clock
  rate of the system in MHz (Million cycles / second).  We make use of
  the x86 RDTSCP instruction.

  This file contains two lab questions, #4 and #5.
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/times.h>
#include "clock.h"

/*
 * Routines for using the cycle counter
 */

// Initialize the cycle counter
static unsigned cyc_hi = 0;
static unsigned cyc_lo = 0;


void access_counter(unsigned *hi, unsigned *lo)
// Set *hi and *lo to the high and low order bits of the cycle
// counter.  Implementation requires assembly code to use the RDTSCP
// instruction.
{
  // CS350c (4): Explain what this code is doing. What is the ``asm''
  // directive? What is the meaning of rdtscp?  What values end up in
  // the hi and lo variables after running this code?
  asm("rdtscp; movl %%edx,%0; movl %%eax,%1"  // Read cycle counter
      : "=r" (*hi), "=r" (*lo)                // and move results to
      : /* No input */                        // the two outputs
      : "%edx", "%eax");
}

void start_counter()
// Record the current value of the cycle counter.
{
    access_counter(&cyc_hi, &cyc_lo);
}

double get_counter()
// Return the number of cycles since the last call to start_counter.
{
    unsigned ncyc_hi, ncyc_lo;
    unsigned hi, lo, borrow;
    double result;

    /* Get cycle counter */
    access_counter(&ncyc_hi, &ncyc_lo);

    // CS350c (5): What is being computed below? ``result'' should end
    // up with the number of cycles that have elapsed since the
    // initial call to start_counter(), but how does it attain that
    // value?  Be sure to explain for what each of the following seven
    // variables are being used:

    //  Variables:  ncyc_hi, ncyc_lo, cyc_hi, cyc_lo, hi, lo, borrow

    lo = ncyc_lo - cyc_lo;
    borrow = lo > ncyc_lo;
    hi = ncyc_hi - cyc_hi - borrow;
    result = (double) hi * (1 << 30) * 4 + lo;
    if (result < 0) {
	fprintf(stderr, "Error: counter returns neg value: %.0f\n", result);
    }
    return result;
}

double mhz(int verbose, int sleeptime)
// Estimate the clock rate by measuring the cycles that elapse while
// sleeping for sleeptime seconds
{
    double rate;

    start_counter();
    sleep(sleeptime);
    rate = get_counter() / (1e6*sleeptime);
    if (verbose)
	printf("Processor clock rate ~= %.1f MHz\n", rate);
    return rate;
}

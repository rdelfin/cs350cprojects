// mem-chase.h

// This is the only file that you may modify for Laboratory 1.

// There are two function defined in this file.  You, as a cs350c
// student, must write C code to initialize the array "mem" so the
// program in "mem-chase.c" runs both as fast as possible and as
// slow as possible.


void init_data_fast( unsigned long int mem[], unsigned long int size ) {
  // Simple, in-order accesses
  unsigned long int i;

  for (i = 0; i < size; i++)
    mem[i] = (i + 1);

  // And now, we overwrite the final entry
  mem[ size - 1 ] = 0;
}

void init_data_slow( unsigned long int mem[], unsigned long int size ) {
  // A place for students to place their work.
}

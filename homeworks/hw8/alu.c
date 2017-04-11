// ALU "gate-level" circuit emulation
// cs350c

#include <stdio.h>
#include <stdlib.h>

typedef unsigned char bit;

struct bits1 {
  bit a;
};

struct bits2 {
  bit a;
  bit b;
};

struct bits3 {
  bit a;
  bit b;
  bit c;
};

struct bits4 {
  bit a;
  bit b;
  bit c;
  bit d;
};

typedef struct bits1 b1;
typedef struct bits2 b2;
typedef struct bits3 b3;
typedef struct bits4 b4;


// Primitive NOT function

b1 not( b1 i ) {
  b1 r;
  bit j = i.a;
  bit k = (!j) & 1;
  r.a = k;
  return( r );
}

// Primitive AND function

b1 and( b1 a, b1 b ) {
  b1 r;
  r.a = a.a & b.a;
  return( r );
}

// Primitive OR function

b1 or( b1 a, b1 b ) {
  b1 r;
  r.a = a.a | b.a;
  return( r );
}

// Defined NAND

b1 nand( b1 a, b1 b ) {
  return( not(and(a, b))  );
}

// Defined XOR function

b1 xor( b1 a, b1 b ) {
  return or(and(a, not(b)), and(not(a), b));
}

// Defined EQUV function

b1 equv( b1 a, b1 b )  {
  return( not(xor(a, b)) );
}

// Defined MUX 2 inputs, 1-bit input
b1 mux_2in_1bit( b1 a, b1 b, b1 s) {
  return or( and(a, s), and(b, not(s)) );
}


// Defined MUX 2 inputs, 4-bit input
b4 mux_2in_4bit( b4 a, b4 b, b1 s ) {
  b4 r;
  b1 aa, ab, ac, ad, ba, bb, bc, bd;
  aa.a = a.a;
  ab.a = a.b;
  ac.a = a.c;
  ad.a = a.d;
  ba.a = b.a;
  bb.a = b.b;
  bc.a = b.c;
  bd.a = b.d;

  r.a = mux_2in_1bit(aa, ba, s).a;
  r.b = mux_2in_1bit(ab, bb, s).a;
  r.c = mux_2in_1bit(ac, bc, s).a;
  r.d = mux_2in_1bit(ad, bd, s).a;

  return r;
}

// Defined MUX 4 inputs, 4-bit input
b4 mux_4in_4bit( b4 a, b4 b, b2 s ) {
  
}

// End of primitives


// Using just the primitives above, implement a four-bit version of
// the y86 ALU.  This ALU should implement the same functionality as
// the ALU in the class textbook, but instead of a 64-bit version, it
// needs only to implement a 4-bit ALU.

struct y86_alu_in {
  bit c_;

  bit a0;
  bit a1;
  bit a2;
  bit a3;

  bit b0;
  bit b1;
  bit b2;
  bit b3;

  bit s0;
  bit s1;
};

struct y86_alu_out {
  bit f0;
  bit f1;
  bit f2;
  bit f3;

  bit of;
  bit zf;
  bit sf;
};


struct y86_alu_out y86_alu_4_bit( struct y86_alu_in i ) {
  struct y86_alu_out r;

  // Solution goes here.

  return( r );
}


int main( int intc, char *argv[], char *env[] ) {

  // inputs
  b1 a, b;

  // outputs
  bit o;
  b1  q;

  // Create case
  a.a = 0;
  b.a = 0;

  q = equv( a, b );
  o = q.a;

  // Print output
  printf( "a= %d, b= %d, z= %d\n", a.a, b.a, o );

  // Next case
  a.a = 1;
  b.a = 0;

  q = equv( a, b );
  o = q.a;

  printf( "a= %d, b= %d, z= %d\n", a.a, b.a, o );

  // Next case
  a.a = 0;
  b.a = 1;


  q = equv( a, b );
  o = q.a;

  printf( "a= %d, b= %d, z= %d\n", a.a, b.a, o );

  // Next case
  a.a = 1;
  b.a = 1;


  q = equv( a, b );
  o = q.a;

  printf( "a= %d, b= %d, z= %d\n", a.a, b.a, o );

  return( 0 );
}

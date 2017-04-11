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

b1 and( b2 i ) {
  b1 r;
  bit j = i.a;
  bit k = i.b;
  bit l = j & k;
  r.a = l;
  return( r );
}

// Primitive OR function

b1 or( b2 i ) {
  b1 r;
  bit j = i.a;
  bit k = i.b;
  bit l = j | k;

//   printf( "j= %d, k= %d, l= %d\n", j, k, l );
  r.a = l;
  return( r );
}

// Defined NAND

b1 nand( b2 i ) {
  b1 r, s;
  r = and( i );
  s = not( r );
  return( s );
}

// Defined XOR function

b1 xor( b2 i ) {

  // This is much more verbose than necessary!  This is shown to give
  // an example of using C-language structures.

  b1 r, s, o, p, a, b, an, bn;
  b2 x, y;

  // Destructure i, place in a and b
  a.a = i.a;
  b.a = i.b;

  // printf("a.a= %d, b.a= %d.\n", a.a, b.a);

  // Compute NOT values of inputs
  an = not( a );
  bn = not( b );

  // printf("an.a= %d, bn.a= %d.\n", an.a, bn.a);

  // Structure x
  x.a = an.a;
  x.b = bn.a;

  // printf("x.a= %d, x.b= %d.\n", x.a, x.b);

  // Compute AND( an, bn );
  s = and( x );

  // printf("s.a= %d.\n", s.a);

  r = and( i );

  // Structure y
  y.a = r.a; // a & b
  y.b = s.a; // a~ & b~

  // printf("y.a= %d, y.b= %d.\n", y.a, y.b);

  // Compute OR
  o = or( y );

  p = not( o );

  return( p );
}

// Defined EQUV function

b1 equv( b2 i )  {
  b1 r, s;

  r = xor( i );
  s = not( r );

  return( s );
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
  bit a, b;
  b2 i;

  // outputs
  bit o;
  b1  q;

  // Create case
  a = 0;
  b = 0;

  // Set inputs to structure
  i.a = a;
  i.b = b;

  q = equv( i );
  o = q.a;

  // Print output
  printf( "a= %d, b= %d, z= %d\n", a, b, o );

  // Next case
  a = 1;
  b = 0;

  i.a = a;
  i.b = b;

  q = equv( i );
  o = q.a;

  printf( "a= %d, b= %d, z= %d\n", a, b, o );

  // Next case
  a = 0;
  b = 1;

  i.a = a;
  i.b = b;

  q = equv( i );
  o = q.a;

  printf( "a= %d, b= %d, z= %d\n", a, b, o );

  // Next case
  a = 1;
  b = 1;

  i.a = a;
  i.b = b;

  q = equv( i );
  o = q.a;

  printf( "a= %d, b= %d, z= %d\n", a, b, o );

  return( 0 );
}

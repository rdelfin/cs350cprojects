// ALU "gate-level" circuit emulation
// cs350c

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

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

struct adder_output1 {
  bit o;
  bit co;
};

struct adder_output4 {
  b4 o;
  bit co;
};

typedef struct adder_output1 addo1;
typedef struct adder_output4 addo4;

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
  return or( and(a, not(s)), and(b, s) );
}

b1 mux_4in_1bit( b1 a, b1 b, b1 c, b1 d, b2 s ) {
  b1 sa, sb;
  sa.a = s.a;
  sb.a = s.b;

  return mux_2in_1bit(mux_2in_1bit(a, b, sa), mux_2in_1bit(c, d, sa), sb);
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
b4 mux_4in_4bit( b4 a, b4 b, b4 c, b4 d, b2 s ) {
  b1 sa, sb;
  sa.a = s.a;
  sb.a = s.b;

  return mux_2in_4bit(mux_2in_4bit(a, b, sa), mux_2in_4bit(c, d, sa), sb);
}

addo1 one_bit_adder( b1 a, b1 b, b1 ci ) {
  addo1 r;
  r.o = xor(a, xor(b, ci)).a;
  r.co = or(and(a, b), or(and(b, ci), and(a, ci))).a;

  return r;
}

addo4 four_bit_adder( b4 a, b4 b, b1 ci ) {

  b1 a1, a2, a3, a4, b1, b2, b3, b4, ci1, ci2, ci3, ci4;
  addo4 r;
  a1.a = a.a;
  a2.a = a.b;
  a3.a = a.c;
  a4.a = a.d;
  b1.a = b.a;
  b2.a = b.b;
  b3.a = b.c;
  b4.a = b.d;
  ci1.a = ci.a;

  addo1 out1 = one_bit_adder( a1, b1, ci1 );
  ci2.a = out1.co;
  addo1 out2 = one_bit_adder( a2, b2, ci2 );
  ci3.a = out2.co;
  addo1 out3 = one_bit_adder( a3, b3, ci3 );
  ci4.a = out3.co;
  addo1 out4 = one_bit_adder( a4, b4, ci4 );

  r.o.a = out1.o;
  r.o.b = out2.o;
  r.o.c = out3.o;
  r.o.d = out4.o;
  r.co = out4.co;

  return r;
}

b4 and4( b4 a, b4 b ) {
  b4 r;
  b1 a1, a2, a3, a4, b1, b2, b3, b4;

  a1.a = a.a;
  a2.a = a.b;
  a3.a = a.c;
  a4.a = a.d;
  b1.a = b.a;
  b2.a = b.b;
  b3.a = b.c;
  b4.a = b.d;
  r.a = and(a1, b1).a;
  r.b = and(a2, b2).a;
  r.c = and(a3, b3).a;
  r.d = and(a4, b4).a;

  return r;
}

b4 xor4( b4 a, b4 b ) {
  b4 r;
  b1 a1, a2, a3, a4, b1, b2, b3, b4;

  a1.a = a.a;
  a2.a = a.b;
  a3.a = a.c;
  a4.a = a.d;
  b1.a = b.a;
  b2.a = b.b;
  b3.a = b.c;
  b4.a = b.d;
  r.a = xor(a1, b1).a;
  r.b = xor(a2, b2).a;
  r.c = xor(a3, b3).a;
  r.d = xor(a4, b4).a;

  return r;
}

b4 not4( b4 a ) {
  b4 r;
  b1 a1, a2, a3, a4;

  a1.a = a.a;
  a2.a = a.b;
  a3.a = a.c;
  a4.a = a.d;
  r.a = not( a1 ).a;
  r.b = not( a2 ).a;
  r.c = not( a3 ).a;
  r.d = not( a4 ).a;

  return r;
}

addo4 four_bit_sub( b4 a, b4 b, b1 ci ) {
  b4 one;
  b1 zero;
  one.a = 1;
  one.b = one.c = one.d = 0;
  zero.a = 0;
   
  addo4 b_neg = four_bit_adder(a, four_bit_adder(not4(b), one, zero).o, ci);
  
  return b_neg;
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

int b4_to_int(b4 a) {
  return a.a | (a.b << 1) | (a.c << 2) | (a.d << 3);
}


struct y86_alu_out y86_alu_4_bit( struct y86_alu_in i ) {
  struct y86_alu_out r;
  
  // Extract data into useful structures
  b4 a, b;
  b1 zero, sa, sb;
  b2 s;
  a.a = i.a0;
  a.b = i.a1;
  a.c = i.a2;
  a.d = i.a3;
  b.a = i.b0;
  b.b = i.b1;
  b.c = i.b2;
  b.d = i.b3;
  zero.a = 0;
  s.a = i.s0;
  s.b = i.s1;
  sa.a = s.a;
  sb.a = s.b;

  // Calculate all operations
  addo4 add = four_bit_adder(a, b, zero);
  addo4 sub = four_bit_sub(a, b, zero);
  b4 and_r = and4(a, b);
  b4 xor_r = xor4(a, b);

  // Choose one using mux and write back to r
  b4 out = mux_4in_4bit(add.o, sub.o, and_r, xor_r, s);
  r.f0 = out.a;
  r.f1 = out.b;
  r.f2 = out.c;
  r.f3 = out.d;


  // Calculate the condition flags (overflow, zero and sign flag)
  b1 sum_of, sub_of;
  sum_of.a = add.co;
  sub_of.a = sub.co;

  b1 out_not_a, out_not_b, out_not_c, out_not_d;
  b4 out_not = not4(out);
  out_not_a.a = out_not.a;
  out_not_b.a = out_not.b;
  out_not_c.a = out_not.c;
  out_not_d.a = out_not.d;

  r.of = mux_2in_1bit(mux_2in_1bit(sum_of, sub_of, sa), zero, sb).a;
  r.zf = and(out_not_a, and(out_not_b, and(out_not_c, out_not_d))).a;
  r.sf = out.d;

  return( r );
}


int main( int argc, char *argv[], char *env[] ) {
  
  // Testing addition:

  int a, b; // a and b as integers
  b2 s;     // Signal is (0,0)
  s.a = s.b = 0;

  int total_tests = pow(2, 4)*pow(2, 4);
  int passed_tests = 0;

  for(a = 0; a < pow(2, 4); a++) {
    for(b = 0; b < pow(2, 4); b++) {
      struct y86_alu_in in;
      in.a0 = !!(a & 1);
      in.a1 = !!(a & 2);
      in.a2 = !!(a & 4);
      in.a3 = !!(a & 8);
      in.b0 = !!(b & 1);
      in.b1 = !!(b & 2);
      in.b2 = !!(b & 4);
      in.b3 = !!(b & 8);
      in.s0 = s.a;
      in.s1 = s.b;
 
      struct y86_alu_out r = y86_alu_4_bit(in);
      int c = r.f0 | (r.f1 << 1) | (r.f2 << 2) | (r.f3 << 3);
      int c_calc = 0xF & a+b;
      int of = !!(0x10 & a+b), zf = !!((0xF&a+b) == 0), sf = !!(0x8 & a+b);

      if(c != c_calc || of != r.of || zf != r.zf || sf != r.sf) {
        printf("%d + %d =\n\t(alu): 0b%d%d%d%d, of: %d, zf: %d, sf: %d\n\t(c):   0b%d%d%d%d, of: %d, zf: %d, sf: %d\n", a, b,
                                                                        r.f3, r.f2, r.f1, r.f0, r.of, r.zf, r.sf,
                                                                        !!(c_calc & 8), !!(c_calc & 4), !!(c_calc & 2), !!(c_calc & 1), of, zf, sf);
      } else
        passed_tests++;
    }
  }

  printf("Passed %d/%d tests for addition\n", passed_tests, total_tests);

  return( 0 );
}


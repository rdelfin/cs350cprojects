
/* y86-pipeline.c                                 Ben Selfridge (2017)

   A C-based simulator for the Y86 PIPE- implementation presented in
   Bryant & O'Hallaron, "Computer Systems: A Programmer's
   Perspective".

   This is a simple, five-stage pipeline for the Y86 ISA. We have
   modeled this simulator as closely as possible to the PIPE- model

   We have left a lot of this simulator unimplemented. As a result of
   this, it does not work correctly!  As an example, the PC increment
   logic simply adds 1 to the current PC; since Y86 has variable-width
   instructions, this is totally incorrect behavior.  It is your job to
   fill in the missing code.  You will need to refer to your textbook
   frequently.

   The code you are to implement is marked by large comments with the
   header "LAB QUESTION".  Every single function must be completed in
   order for the pipeline to work correctly.  You should test your
   pipeline extensively by writing both small, simple programs (to
   test individual instructions and hazard scenarios) and larger
   programs (like your quicksort and fibonacci implementations)
   presented in the book (the one with no feedback paths).
 */

/* NOTE TO STUDENTS:
 *
 * We are aware that the solutions to many of the lab questions below
 * are in the book as solutions to the practice problems.  However,
 * many of those solutions are for the non-pipelined Y86.  We are
 * looking for the solutions for the Y86 PIPE- microprocessor.  You are
 * free to use these solutions to help you figure out how to do the
 * problem, but some of these solutions are incorrect for the
 * pipelined machine.  But, the non-pipelined solutions are a good place
 * to start.
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

/* To change the real memory size, modify the constants below. */
#define IMEMSIZE 0x10000
#define DMEMSIZE 0x10000

/* Do not modify these constants. */
#define NUMREGS 15

#define HALT    0
#define NOP     1
#define RRMOVQ  2
#define IRMOVQ  3
#define RMMOVQ  4
#define MRMOVQ  5
#define OPQ     6
#define JXX     7
#define CALL    8
#define RET     9
#define PUSHQ  10
#define POPQ   11

#define AOK 1
#define HLT 2
#define ADR 3
#define INS 4

#define RRSP   4
#define RNONE 15

typedef uint64_t u64;
typedef int64_t i64;
typedef unsigned char byte;

/* PIPELINE LATCH STRUCTS */

typedef struct Fetch_struct {
  u64 predPC;
} Fetch;

typedef struct Decode_struct {
  int stat;
  int icode;
  int ifun;
  int rA;
  int rB;
  u64 valC;
  u64 valP;
} Decode;

typedef struct Execute_struct {
  int stat;
  int icode;
  int ifun;
  u64 valC;
  u64 valA;
  u64 valB;
  int dstE;
  int dstM;
  int srcA;
  int srcB;
} Execute;

typedef struct Memory_struct {
  int stat;
  int icode;
  int Cnd;
  u64 valE;
  u64 valA;
  int dstE;
  int dstM;
} Memory;

typedef struct Writeback_struct {
  int stat;
  int icode;
  u64 valE;
  u64 valM;
  int dstE;
  int dstM;
} Writeback;

/* STATE OF Y86 */

byte imem[IMEMSIZE];
byte dmem[DMEMSIZE];

/* Condition codes */
int zf, sf, of;

/* Program status (AOK, HLT, ADR, INS) */
int stat;

/* The latches for the current cycle. */
u64 rgf[NUMREGS];
Fetch f;
Decode d;
Execute e;
Memory m;
Writeback w;

/* The latches for the next cycle. */
Fetch f_next;
Decode d_next;
Execute e_next;
Memory m_next;
Writeback w_next;

/* HELPER STATE */
/* Keeps track of number of cycles we have run. */
int cycle_count;
/* Ability to add breakpoints */
#define MAXBREAKPOINTS 100
int num_breakpoints;
u64 breakpoints[MAXBREAKPOINTS];

/* UTILITY FUNCTIONS */
/* Convert an array of 8 bytes into an 64-bit integer. */
u64 bytes_to_u64(byte *bytes) {
  u64 val = 0;

  val |=  (bytes[7] & 0xFF);
  val <<= 8;
  val |=  (bytes[6] & 0xFF);
  val <<= 8;
  val |=  (bytes[5] & 0xFF);
  val <<= 8;
  val |=  (bytes[4] & 0xFF);
  val <<= 8;
  val |=  (bytes[3] & 0xFF);
  val <<= 8;
  val |=  (bytes[2] & 0xFF);
  val <<= 8;
  val |=  (bytes[1] & 0xFF);
  val <<= 8;
  val |=  (bytes[0] & 0xFF);

  return val;
}

int breakpoint(u64 select_pc) {
  int i;
  for (i = 0; i < num_breakpoints; i++)
    if (select_pc == breakpoints[i]) return 1;
  return 0;
}

int add_breakpoint(u64 pc) {
  if (num_breakpoints < MAXBREAKPOINTS) {
    breakpoints[num_breakpoints++] = pc;
    return 1; /* success */
  }
  return 0; /* failure */
}

void show_breakpoints() {
  int i;
  printf("Breakpoints:\n");
  for (i = 0; i < num_breakpoints; i++)
    printf("  %lld\n", breakpoints[i]);
  printf("\n");
}

void delete_breakpoints() {
  num_breakpoints = 0;
}

/* DETECTING DEPENDENCIES/HAZARDS */

int reads_rA(int icode) {
  return (icode == RRMOVQ || icode == RMMOVQ ||
	  icode == OPQ || icode == PUSHQ);
}

int reads_rB(int icode) {
  return (icode == OPQ || icode == RMMOVQ || icode == MRMOVQ);
}

int reads_rsp(int icode) {
  return (icode == PUSHQ || icode == POPQ ||
	  icode == CALL || icode == RET);
}

/* LAB QUESTION: reg_data_dependency
 * Difficulty: 5/5
 *
 * As a part of the pipeline control logic, we need to be able to
 * detect whether we have a register data dependency between the
 * decode stage (which is the part of the pipeline that reads from the
 * register file) and any of the instructions in the later stages
 * (execute, memory, writeback). If the d latch is going to read a
 * register rX, but the e, m, or w latches are going to write to that
 * same register, we stall the instruction in the decode register.
 *
 * Fill in the body of the function below to detect whether the decode
 * stage is reading from a register that is going to be written to by
 * an instruction further down the pipeline (in the e, m, or w
 * latches). We have provided three functions above, reads_rA,
 * reads_rB, and reads_rsp, which you may use to write your
 * function. Do not modify the signature of this function; your
 * solution should take 0 arguments, and return a 1 if there is a
 * register data dependency in the pipeline, and 0 otherwise.
 *
 * To figure this problem out, you will need a full understanding of
 * Section 4.5 in the book, particularly 4.5.5.
 */
int reg_data_dependency() {
  /* SOLUTION STARTS HERE */

  return 0; /* change this! */

  /* SOLUTION ENDS HERE */
}

/* LAB QUESTION: mispredicted_branch
 * Difficulty: 3/5
 *
 * As a part of the pipeline control logic, we need to be able to
 * detect whether we mispredicted a branch. Since our simulator uses
 * the "branch taken" branch prediction policy (we always take every
 * conditional jump), this means that a branch has been mispredicted
 * if it was never supposed to be taken in the first place.
 *
 * Fill in the body of the function below to determine, using only the
 * pipeline variables f, d, e, m, and w, whether we have mispredicted
 * a branch. Do not modify the signature of this function; your
 * solution should take 0 arguments, and return a 1 if a branch has
 * been mispredicted, and a 0 otherwise.
 *
 * To figure this problem out, first ask yourself the following
 * question: Which latch of the pipeline contains information about
 * whether the instruction causes a branch or not?
 *
 * To do this problem, you will need a full understanding of Section
 * 4.5 in the book, particularly 4.5.4 and 4.5.5, subsection "Avoiding
 * Control Hazards."
 */
int mispredicted_branch() {
  /* SOLUTION STARTS HERE */

  return 0; /* change this! */

  /* SOLUTION ENDS HERE */
}

/* LAB QUESTION: ret_d, ret_e, ret_m
 * Difficulty: 2/5
 *
 * As a part of the pipeline control logic, we need to detect if there
 * is a RET instruction in d, e, or m latches. If there is one, we do
 * not bother to fetch the next instruction until the RET instruction
 * has reached the w latch, at which point the fetch stage will use
 * the value retrieved from memory as the next PC.
 *
 * Fill in the body of each of the following three functions, to
 * determine whether there is a RET instruction in the d, e, and m
 * latches, respectively.
 */
int ret_d() {
  /* SOLUTION STARTS HERE */

  return 0; /* change this! */

  /* SOLUTION ENDS HERE */
}
int ret_e() {
  /* SOLUTION STARTS HERE */

  return 0; /* change this! */

  /* SOLUTION ENDS HERE */
}
int ret_m() {
  /* SOLUTION STARTS HERE */

  return 0; /* change this! */

  /* SOLUTION ENDS HERE */
}

/* MEMORY ACCESS */

/*
 * This function reads 10 bytes from the instruction memory at a
 * specified address. It does not return a value; rather, it stores an
 * error value in a third argument, addressed by the imem_error
 * pointer variable.
 */
void read_imem(u64 addr,           /* address to read
				      (unsigned 64-bit value) */
	       byte *data,         /* array of 10 bytes to
				      store output */
	       byte *imem_error)   /* signal indicating some
				      kind of memory error */
{
  /* First we check that the access is within the bounds of our physical memory. */
  if (addr+10 > IMEMSIZE) {
    if (imem_error) *imem_error = 1;
    return;
  }

  /* Memory reads 10 bytes at a time. */
  for (int i=0; i < 10; i++, addr++) {
    data[i] = imem[addr];
  }

  if (imem_error) *imem_error = 0;
}

/* LAB QUESTION: read_dmem
 * Difficulty: 1/5
 *
 * Provide the code for the read_dmem function. It should look exactly
 * like the read_imem function, above, with a few minor changes.
 *
 * IMPORTANT: The instruction memory reads 10 bytes at a time, but the
 * data memory only reads 8. You will need to take this into account
 * when modifying the read_imem code to complete your solution.
 */
/*
 * This function reads 8 bytes from the data memory at a specified
 * address. It does not return a value; rather, it stores an error
 * value in a third argument, addressed by the imem_error pointer
 * variable.
 */
void read_dmem(u64 addr,           /* address to read
				      (unsigned 64-bit value) */
	       byte *data,         /* array of 8 bytes to
				      store output */
	       byte *dmem_error)   /* signal indicating some
				      kind of memory error */
{
  /* SOLUTION STARTS HERE */

  /* SOLUTION ENDS HERE */
}

void write_dmem(u64 addr,            /* address to write (unsigned 64-bit value) */
		u64 data,            /* data to write (64-bit signed integer) */
		byte *mem_error) /* signal indicating some kind of memory error */
{
  /* First we check that the access is within the bounds of our physical memory. */
  if (addr+8 > DMEMSIZE) {
    if (mem_error) *mem_error = 1;
    return;
  }

  dmem[addr]   = (byte) data & 0xFF;
  dmem[addr+1] = (byte) (data >>  8) & 0xFF;
  dmem[addr+2] = (byte) (data >> 16) & 0xFF;
  dmem[addr+3] = (byte) (data >> 24) & 0xFF;
  dmem[addr+4] = (byte) (data >> 32) & 0xFF;
  dmem[addr+5] = (byte) (data >> 40) & 0xFF;
  dmem[addr+6] = (byte) (data >> 48) & 0xFF;
  dmem[addr+7] = (byte) (data >> 56) & 0xFF;

  if (mem_error) *mem_error = 0;
}

/* FETCH STAGE */

/* LAB QUESTION: f_select_pc
 * Difficulty: 3/5
 *
 * Below is the function that models the PC Selection logic, which is
 * part of the fetch stage. We have three potential sources for the
 * next PC, all of which were computed in the previous cycle: the
 * value in f.predPC, the value in m.valA, and the value in w.valM.
 *
 * Usually, we use f.predPC, but there are specific situations where
 * the value in f.predPC will be ignored and instead we will use
 * either m.valA or w.valM.
 *
 * Write the body of the function below to correctly determine which
 * location to fetch, given the six inputs provided by the
 * function. DO NOT USE ANY OTHER VALUES TO DETERMINE YOUR SOLUTION!
 * You are not allowed to refer to any global variables in this
 * function.
 *
 * For help answering this question, refer to Section 4.5.7, "PC
 * Selection and Fetch Stage". The logic for this circuit is given at
 * the bottom of page 448; so essentially we are asking you to
 * translate this into C code.
 */
u64 f_select_pc(int f_predPC,
		int m_icode, int m_Cnd, int m_valA,
		int w_icode, int w_valM) {
  /* SOLUTION STARTS HERE */
  /* DO NOT refer to any global variables!!! Only use the arguments to
     the function. */

  return f_predPC; /* change this! */

  /* SOLUTION ENDS HERE */
}

int f_icode(byte *bytes, int imem_error) {
  return (bytes[0] & 0xF0) >> 4;
}

int f_ifun(byte *bytes) {
  return bytes[0] & 0x0F;
}

int f_rA(byte *bytes, int need_regids) {
  if (need_regids) return (bytes[1] & 0xF0) >> 4;
  return RNONE;
}

int f_rB(byte *bytes, int need_regids) {
  if (need_regids) return bytes[1] & 0x0F;
  return RNONE;
}

u64 f_valC(byte *bytes, int need_regids) {
  if (need_regids) return bytes_to_u64(&bytes[2]);
  return bytes_to_u64(&bytes[1]);
}

/* LAB QUESTION: f_instr_valid
 * Difficulty: 4/5
 *
 * In the book (pg. 405), the instr_valid circuit takes only the icode
 * as input. However, in order to really tell if an instruction is
 * valid in the Y86 ISA, we need to consider more inputs. In other
 * words, the book messed it up.
 *
 * Below is the function used to model the instr_valid circuit as
 * given in the book. Modify it in order to detect ALL the invalid
 * instructions. You will need to add at least one argument to the
 * function, and you will certainly need to modify the body of the
 * function quite a bit.
 *
 * DO NOT USE ANY GLOBAL VARIABLES INSIDE THIS FUNCTION! Any
 * information you need to compute the value of this function should
 * be added as explicit arguments.
 *
 * IMPORTANT: When you are done with this function, you will need to
 * modify the code in the fetch() function that calls it (since you
 * are adding arguments to this function, you need to supply those
 * arguments!) This is marked explicitly as a lab question.
 *
 * To do this problem, look carefully at the Y86 ISA encoding
 * described in the book. You must consider every scenario where the
 * bits in the "instruction" do not correspond to an actual
 * instruction in the ISA.
 */
int f_instr_valid(int icode, int ifun, int rA, int rB) {
  int valid_icode = (icode >= 0x0) && (icode <= 0xB);
  int valid_ifun = (ifun == 0); // Most common case
  int valid_rA = 1;
  int valid_rB = 1;

  if(valid_icode) {
    switch(icode) {
      case HALT:    // All cases covered.
	break;

      case NOP:     // All cases covered
	break;

      case RRMOVQ:  // Check registers, allow more ifun
	valid_rA = (rA >= 0x0 && rA < 0xF);
	valid_rB = (rB >= 0x0 && rB < 0xF);
	valid_ifun = (ifun >= 0x0 && ifun <= 0x6);
	break;

      case IRMOVQ:  // Check only registers
	valid_rA = (rA == 0xF);
	valid_rB = (rB >= 0x0 && rB < 0xF);
	break;

      case RMMOVQ:  // Check registers only
	valid_rA = (rA >= 0x0 && rA < 0xF);
	valid_rB = (rB >= 0x0 && rB < 0xF);
	break;

      case MRMOVQ:  // Check registers only
	valid_rA = (rA >= 0x0 && rA < 0xF);
	valid_rB = (rB >= 0x0 && rB < 0xF);
	break;

      case OPQ:     // Check registers and function code
	valid_rA = (rA >= 0x0 && rA < 0xF);
	valid_rB = (rB >= 0x0 && rB < 0xF);
	valid_ifun = (ifun >= 0x0 && ifun <= 0x3);
	break;

      case JXX:	    // Check function code only
	valid_ifun = (ifun >= 0x0 && ifun <= 0x6);
	break;

      case CALL:    // All cases covered
	break;

      case RET:     // All cases covered
	break;

      case PUSHQ:   // Check registers only
	valid_rA = (rA >= 0x0 && rA < 0xF);
	valid_rB = (rB == 0xF);
	break;

      case POPQ:    // Check registers only
	valid_rA = (rA >= 0x0 && rA < 0xF);
	valid_rB = (rB == 0xF);
	break;
    }
  }
  return valid_icode && valid_ifun && valid_rA && valid_rB;
}

int f_need_regids(int f_icode) {
  return
    f_icode == RRMOVQ ||
    f_icode == IRMOVQ ||
    f_icode == RMMOVQ ||
    f_icode == MRMOVQ ||
    f_icode == OPQ ||
    f_icode == PUSHQ ||
    f_icode == POPQ;
}

/* LAB QUESTION: f_need_valC
 * Difficulty: 2/5
 *
 * Below is the function that models the need_valC logic, which
 * computes whether the instruction being fetched uses a constant
 * value. (This information is needed in the pc increment logic to
 * determine what the next predicted PC will be.) Right now, the
 * function incorrectly returns 0 for every icode.
 *
 * Provide the body for this function by returning 1 if the icode
 * requires a constant value. DO NOT USE ANY GLOBAL VARIABLES!
 *
 * This question corresponds to Practice Problem 4.19. Pages 405-406
 * should be helpful for this problem.
 */
int f_need_valC(int icode) {
  /* SOLUTION STARTS HERE */
  /* DO NOT refer to any global variables!!! Only use the arguments to
     the function. */

  return 0; /* change this! */

  /* SOLUTION ENDS HERE */
}

/* LAB QUESTION: f_pc_increment
 * Difficulty: 2/5
 *
 * Below is the function that models the pc_increment logic, which
 * computes how much to increment the PC by to fetch the (likely) next
 * instruction. Right now, the function incorrectly increments the
 * selected PC by one, regardless of the icode of the instruction
 * being fetched.
 *
 * Provide the body for this function by returning the value of
 * select_pc, incremented by the width of the current instruction (to
 * be computed from the inputs need_valC and need_regids).
 *
 * Your solution should implement the textual description of this
 * function described on page 406.
 */
int f_pc_increment(int select_pc, int need_valC, int need_regids) {
  return select_pc + 1 + (need_valC ? 8 : 0) + (need_regids ? 1 : 0);
}

int f_predict_pc(int f_icode, int f_valC, int f_pc_increment) {
  if (f_icode == JXX || f_icode == CALL)
    return f_valC; /* predict taken for jumps */
  return f_pc_increment;
}

int f_stat(int icode, int f_instr_valid, int imem_error) {
  if (imem_error) return ADR;
  else if (!f_instr_valid) return INS;
  else if (icode == HALT) return HLT;
  else return AOK;
}

void bubble_d() {
  d_next.stat = 1;
  d_next.icode = 0;
  d_next.ifun = 0;
  d_next.rA = RNONE;
  d_next.rB = RNONE;
  d_next.valC = 0;
  d_next.valP = 0;
}

int fetch() {
  /* Don't report breakpoints until it is time to fetch */
  if (w.stat != AOK || m.stat != AOK) return 0;
  if (!mispredicted_branch()) {
    if (ret_m()) {
      bubble_d();
      return 0;
    }
    if (e.stat != AOK) return 0;
    if (ret_e()) {
      bubble_d();
      return 0;
    }
    if (d.stat != AOK) {
      bubble_d();
      return 0;
    }
    if (reg_data_dependency()) return 0;
    if (ret_d()) {
      bubble_d();
      return 0;
    }
  }

  u64 select_pc = f_select_pc(f.predPC,
			      m.icode, m.Cnd, m.valA,
			      w.icode, w.valM);

  byte inst[10], imem_error;

  /* read from instruction memoery */
  read_imem(select_pc, inst, &imem_error);

  /* compute icode, ifun */
  int icode = f_icode(inst, imem_error);
  int ifun  = f_ifun(inst);

  /* compute need_regids and need_valc */
  int need_regids = f_need_regids(icode);
  int need_valC   = f_need_valC(icode);

  /* compute rA, rB, valC */
  int rA    = f_rA(inst, need_regids);
  int rB    = f_rB(inst, need_regids);
  int valC  = f_valC(inst, need_regids);

  /* LAB QUESTION: instr_valid (part 2)
   * Difficulty: 1/5
   *
   * Modify the line below to call f_instr_valid correctly, using your
   * modified implementation of this function.
   */
  int instr_valid = f_instr_valid(icode, ifun, rA, rB); /* change this! */

  /* compute PC increment */
  int pc_increment = f_pc_increment(select_pc, need_valC, need_regids);

  /* compute predicted PC */
  int predict_pc = f_predict_pc(icode, valC, pc_increment);

  /* compute stat */
  int stat = f_stat(icode, instr_valid, imem_error);

  /* Latch next values for the fetch and decode latches */
  f_next.predPC = predict_pc;
  d_next.stat   = stat;
  d_next.icode  = icode;
  d_next.ifun   = ifun;
  d_next.rA     = rA;
  d_next.rB     = rB;
  d_next.valC   = valC;
  d_next.valP   = pc_increment;

  /* Detect breakpoint! */
  if (breakpoint(select_pc)) return 1;
  else return 0;
}

/* DECODE STAGE */

int d_srcA(int d_icode, int d_ra) {
  if (d_icode == RRMOVQ || d_icode == RMMOVQ ||
      d_icode == OPQ || d_icode == PUSHQ)
    return d_ra;
  if (d_icode == POPQ || d_icode == RET)
    return RRSP;
  return RNONE;
}

/* LAB QUESTION: d_srcB
 * Difficulty: 2/5
 *
 * Below is the function that models the d_srcB logic, which computes
 * which register (if any) to read from the register file, whose value
 * will be placed in the e.valB field in the execute latch. Right now,
 * this function incorrectly returns RNONE in all cases.
 *
 * Provide the body for this function. You should either return d_rb,
 * RRSP, or RNONE if the instruction doesn't need a value for rB.
 *
 * This question corresponds to Practice Problem 4.20. You will need
 * to refer to figures 4.18 to 4.21 to complete this problem.
 */
int d_srcB(int d_icode, int d_rb) {
  /* SOLUTION STARTS HERE */
  /* DO NOT use any global variables! */

  return RNONE; /* change this! */

  /* SOLUTION ENDS HERE */
}

int d_dstE(int d_icode, int d_rb) {
  if (d_icode == RRMOVQ) return d_rb; /* conditional move included */
  if (d_icode == IRMOVQ || d_icode == OPQ)
    return d_rb;
  if (d_icode == PUSHQ || d_icode == POPQ ||
      d_icode == CALL || d_icode == RET)
    return RRSP;
  return RNONE;
}

/* LAB QUESTION: d_dstM
 * Difficulty: 2/5
 *
 * Below is the function that models the d_dstM logic, which computes
 * which register (if any) where the value read from memory will be
 * placed. Right now, this function incorrectly returns RNONE in all
 * cases.
 *
 * Provide the body for this function. You should either return d_ra
 * or RNONE if the instruction doesn't need a value for rB.
 *
 * This question corresponds to Practice Problem 4.21. You will need
 * to refer to figures 4.18 to 4.21 to complete this problem.
 */
int d_dstM(int d_icode, int d_ra) {
  /* SOLUTION STARTS HERE */
  /* DO NOT use any global variables! */

  return RNONE; /* change this! */

  /* SOLUTION ENDS HERE */
}

u64 d_select_a(int d_icode, u64 d_rvalA, u64 d_valP) {
  if (d_icode == CALL || d_icode == JXX)
    return d_valP;
  return d_rvalA;
}

void bubble_e() {
  e_next.stat = 1;
  e_next.icode = NOP;
  e_next.ifun = 0;
  e_next.valC = 0;
  e_next.valA = 0;
  e_next.valB = 0;
  e_next.dstE = RNONE;
  e_next.dstM = RNONE;
  e_next.srcA = RNONE;
  e_next.srcB = RNONE;
}

void decode() {
  if (w.stat != AOK || m.stat != AOK) return;
  if (mispredicted_branch()) {
    bubble_e();
    return;
  }
  if (e.stat != AOK) {
    bubble_e();
    return;
  }
  if (reg_data_dependency()) {
    bubble_e();
    return;
  }

  /* compute srcA, srcB, dstE, dstM */
  int srcA = d_srcA(d.icode, d.rA);
  int srcB = d_srcB(d.icode, d.rB);
  int dstE = d_dstE(d.icode, d.rB);
  int dstM = d_dstM(d.icode, d.rA);

  /* read from register file */
  u64 d_rvalA = srcA == RNONE ? 0 : rgf[srcA];
  u64 d_rvalB = srcB == RNONE ? 0 : rgf[srcB];

  /* compute valA */
  u64 select_a = d_select_a(d.icode, d_rvalA, d.valP);

  /* Latch next values for the execute latch */
  e_next.stat = d.stat;
  e_next.icode = d.icode;
  e_next.ifun = d.ifun;
  e_next.valC = d.valC;
  e_next.valA = select_a;
  e_next.valB = d_rvalB;
  e_next.dstE = dstE;
  e_next.dstM = dstM;
  e_next.srcA = srcA;
  e_next.srcB = srcB;
}

/* EXECUTE STAGE */
u64 e_alu_a(int icode, u64 valC, u64 valA) {
  if (icode == RRMOVQ || icode == OPQ) return valA;
  if (icode == IRMOVQ || icode == RMMOVQ || icode == MRMOVQ)
    return valC;
  if (icode == CALL || icode == PUSHQ) return -8;
  if (icode == RET || icode == POPQ) return 8;

  /* Other instructions don't use the ALU */
  return 0;
}

/* LAB QUESTION: e_alu_b
 * Difficulty: 3/5
 *
 * Below is the function that models the alu_b logic, which computes
 * which value to provide as the second argument for the ALU. Right
 * now, it incorrectly passes valB in all cases.
 *
 * Provide the body for this function. You should either return valB
 * or 0.
 *
 * This question corresponds to Practice Problem 4.23. You will need
 * to refer to figures 4.18 to 4.21 to complete this problem.
 */
u64 e_alu_b(int icode, u64 valB) {
  /* SOLUTION STARTS HERE */
  /* DO NOT use any global variables! */

  return valB; /* change this! */

  /* SOLUTION ENDS HERE */
}

int e_set_cc(int icode) {
  return icode == OPQ;
}

int e_alu_fun(int icode, int ifun) {
  if (icode == OPQ) return ifun;

  /* Everything else either does an add, or nothing */
  return 0;
}

/* Uses the zf, of, and sf from the previous cycle(s), not the one
   being computer in this cycle (if any). */
int e_cond(int ifun, int zf, int of, int sf) {
  switch(ifun) {
  case 0: /* unconditional */
    return 1;
  case 1: /* le */
    return (sf ^ of) || zf;
  case 2: /* l  */
    return sf ^ of;
  case 3: /* e  */
    return zf;
  case 4: /* ne */
    return !zf;
  case 5: /* ge */
    return !(sf ^ of);
  case 6: /* g  */
    return !(sf ^ of) && !zf;
  }

  /* Should never happen */
  return 0;
}

u64 e_alu(u64 a, u64 b, int alu_fun,
	  int *zf, int *of, int *sf) {
  u64 result;
  switch(alu_fun) {
  case 0:
    result = b + a;
    break;
  case 1:
    a = -a; /* change sign of a */
    result = b + a;
    break;
  case 2:
    result = b & a;
    break;
  case 3:
    result = b ^ a;
    break;
  default:
    /* Should never happen */
    result = 0;
  }

  /* LAB QUESTION: condition codes
   * Difficulty: 2/5
   *
   * We set the condition codes below. Right now, they are all
   * incorrectly set to 0, regardless of the result and the two input
   * operands.
   *
   * Fix this code so that ZF, SF, and OF are all set correctly.
   * Consult Page 201 for information on how to implement these
   * codes. Your solution should only refer to variables result, a,
   * and b.
   *
   * IMPORTANT: keep in mind that result, a, and b are all unsigned
   * values in C, so they are always interpreted as positive with
   * respect to the C comparison operators <, <=, >, >=.
   */
  /* SOLUTION STARTS HERE */

  *zf = 0;
  *sf = 0;
  *of = 0;

  /* SOLUTION ENDS HERE */

  return result;
}

int e_dstE(int dstE, int icode, int cond) {
  if (icode != 2 || cond == 1) {
    return dstE;
  }
  return RNONE;
}

void bubble_m() {
  m_next.stat = 1;
  m_next.icode = NOP;
  m_next.Cnd = 1;
  m_next.valE = 0;
  m_next.valA = 0;
  m_next.dstE = RNONE;
  m_next.dstM = RNONE;
}

void execute() {
  /* Hazard control */
  if (w.stat != AOK) return;
  if (m.stat != AOK) {
    bubble_m();
    return;
  }
  if (mispredicted_branch()) {
    bubble_m();
    return;
  }

  /* compute aluA, aluB */
  u64 aluA = e_alu_a(e.icode, e.valC, e.valA);
  u64 aluB = e_alu_b(e.icode, e.valB);

  /* compute set_cc */
  int set_cc = e_set_cc(e.icode);

  /* compute alu_fun */
  int alu_fun = e_alu_fun(e.icode, e.ifun);

  /* compute alu result */
  int zf_next, of_next, sf_next;
  u64 alu = e_alu(aluA, aluB, alu_fun, &zf_next, &of_next, &sf_next);

  /* compute cond */
  int cond = e_cond(e.ifun, zf, of, sf);

  /* compute new dstE */
  int dstE = e_dstE(e.dstE, e.icode, cond);

  /* Latch next values for memory latch and condition codes */
  m_next.stat = e.stat;
  m_next.icode = e.icode;
  m_next.Cnd = cond;
  m_next.valE = alu;
  m_next.valA = e.valA;
  m_next.dstE = dstE;
  m_next.dstM = e.dstM;

  if (set_cc) {
    zf = zf_next;
    of = of_next;
    sf = sf_next;
  }
}

/* MEMORY STAGE */

u64 m_addr(int icode, u64 valE, u64 valA) {
  if (icode == RMMOVQ || icode == PUSHQ ||
      icode == CALL || icode == MRMOVQ)
    return valE;
  if (icode == POPQ || icode == RET)
    return valA;
  return 0; /* Other instructions don't need address */
}

int m_mem_read(int icode) {
  return (icode == MRMOVQ || icode == RET || icode == POPQ);
}

/* LAB QUESTION: m_mem_write
 * Difficulty: 2/5
 *
 * Below is the function that models the mem_write circuit, which
 * outputs 1 if the instruction in the m latch is writing to memory,
 * and 0 otherwise. Right now, it incorrectly returns 0 in all cases.
 *
 * Provide the body for this function.
 *
 * This question corresponds to Practice Problem 4.26.
 */
int m_mem_write(int icode) {
  return 0;
}

int m_stat(int stat, int dmem_error) {
  if (stat != 1) return stat;
  if (dmem_error) return ADR;
  return stat;
}

void memory() {
  /* Hazard control */
  if (w.stat != AOK) return;

  /* compute addr */
  u64 addr = m_addr(m.icode, m.valE, m.valA);

  /* compute mem_read and mem_write */
  int mem_read = m_mem_read(m.icode);
  int mem_write = m_mem_write(m.icode);

  /* perform memory operation */
  byte data[8];
  byte dmem_error = 0;
  if (mem_read) {
    read_dmem(addr, data, &dmem_error);
  } else if (mem_write) {
    write_dmem(addr, m.valA, &dmem_error);
  }

  /* compute stat */
  int stat = m_stat(m.stat, dmem_error);

  /* Latch next values for writeback latch */
  w_next.stat = stat;
  w_next.icode = m.icode;
  w_next.valE = m.valE;
  w_next.valM = mem_read ? bytes_to_u64(data) : 0; /* 0 if no read */
  w_next.dstE = m.dstE;
  w_next.dstM = m.dstM;
}

/* WRITEBACK STAGE */

void writeback() {
  /* Update stat */
  stat = w.stat;

  /* If the machine is about to halt, don't write to the register
     file! */
  if (w.stat != AOK) return;

  /* Perform writes to register file */
  if (w.dstE != RNONE)
    rgf[w.dstE] = w.valE;
  if (w.dstM != RNONE)
    rgf[w.dstM] = w.valM;

  /* Done */
}

/* Cycle machine */
/* return 1 if we hit a breakpoint */
int cycle() {
  /* Don't report a breakpoint if the machine is halted */
  if (stat != AOK) return 0; /* if halted, return */

  int breakpoint = fetch();
  decode();
  execute();
  memory();
  writeback();

  f = f_next;
  d = d_next;
  e = e_next;
  m = m_next;
  w = w_next;

  ++cycle_count;

  return breakpoint;
}

/* DUMP FUNCTIONS */
void rgfdump() {
  printf("stat = %d\n", stat);
  printf("zf = %d, of = %d, sf = %d\n\n", zf, of, sf);

  printf("r00 [rax] = %10lld (0x%.2llx)\n", rgf[0], rgf[0]);
  printf("r01 [rcx] = %10lld (0x%.2llx)\n", rgf[1], rgf[1]);
  printf("r02 [rdx] = %10lld (0x%.2llx)\n", rgf[2], rgf[2]);
  printf("r03 [rbx] = %10lld (0x%.2llx)\n", rgf[3], rgf[3]);
  printf("r04 [rsp] = %10lld (0x%.2llx)\n", rgf[4], rgf[4]);
  printf("r05 [rbp] = %10lld (0x%.2llx)\n", rgf[5], rgf[5]);
  printf("r06 [rsi] = %10lld (0x%.2llx)\n", rgf[6], rgf[6]);
  printf("r07 [rdi] = %10lld (0x%.2llx)\n", rgf[7], rgf[7]);
  printf("r08       = %10lld (0x%.2llx)\n", rgf[8], rgf[8]);
  printf("r09       = %10lld (0x%.2llx)\n", rgf[9], rgf[9]);
  printf("r10       = %10lld (0x%.2llx)\n", rgf[10], rgf[10]);
  printf("r11       = %10lld (0x%.2llx)\n", rgf[11], rgf[11]);
  printf("r12       = %10lld (0x%.2llx)\n", rgf[12], rgf[12]);
  printf("r13       = %10lld (0x%.2llx)\n", rgf[13], rgf[13]);
  printf("r14       = %10lld (0x%.2llx)\n\n", rgf[14], rgf[14]);
}

void pipedump() {
  printf("Cycle count = %d\n\n", cycle_count);

  printf("f.predPC  = %10lld\n\n", f.predPC);

  printf("d.stat    = %10d (0x%.2x)\n", d.stat, d.stat);
  printf("d.icode   = %10d (0x%.2x)\n", d.icode, d.icode);
  printf("d.ifun    = %10d (0x%.2x)\n", d.ifun, d.ifun);
  printf("d.rA      = %10d (0x%.2x)\n", d.rA, d.rA);
  printf("d.rB      = %10d (0x%.2x)\n", d.rB, d.rB);
  printf("d.valC    = %10lld (0x%.2llx)\n", d.valC, d.valC);
  printf("d.valP    = %10lld (0x%.2llx)\n\n", d.valP, d.valP);

  printf("e.stat    = %10d (0x%.2x)\n", e.stat, e.stat);
  printf("e.icode   = %10d (0x%.2x)\n", e.icode, e.icode);
  printf("e.ifun    = %10d (0x%.2x)\n", e.ifun, e.ifun);
  printf("e.valC    = %10lld (0x%.2llx)\n", e.valC, e.valC);
  printf("e.valA    = %10lld (0x%.2llx)\n", e.valA, e.valA);
  printf("e.valB    = %10lld (0x%.2llx)\n", e.valB, e.valB);
  printf("e.dstE    = %10d (0x%.2x)\n", e.dstE, e.dstE);
  printf("e.dstM    = %10d (0x%.2x)\n", e.dstM, e.dstM);
  printf("e.srcA    = %10d (0x%.2x)\n", e.srcA, e.srcA);
  printf("e.srcB    = %10d (0x%.2x)\n\n", e.srcB, e.srcB);

  printf("m.stat    = %10d (0x%.2x)\n", m.stat, m.stat);
  printf("m.icode   = %10d (0x%.2x)\n", m.icode, m.icode);
  printf("m.Cnd     = %10d (0x%.2x)\n", m.Cnd, m.Cnd);
  printf("m.valE    = %10lld (0x%.2llx)\n", m.valE, m.valE);
  printf("m.valA    = %10lld (0x%.2llx)\n", m.valA, m.valA);
  printf("m.dstE    = %10d (0x%.2x)\n", m.dstE, m.dstE);
  printf("m.dstM    = %10d (0x%.2x)\n\n", m.dstM, m.dstM);

  printf("w.stat    = %10d (0x%.2x)\n", w.stat, w.stat);
  printf("w.icode   = %10d (0x%.2x)\n", w.icode, w.icode);
  printf("w.valE    = %10lld (0x%.2llx)\n", w.valE, w.valE);
  printf("w.valM    = %10lld (0x%.2llx)\n", w.valM, w.valM);
  printf("w.dstE    = %10d (0x%.2x)\n", w.dstE, w.dstE);
  printf("w.dstM    = %10d (0x%.2x)\n\n", w.dstM, w.dstM);

}

void imemdump(int start, int end) {
  int i;
  for (i = start; i <= end; i++) {
    if (i < 0 || i >= IMEMSIZE) {
      printf("Bad location: %d\n", i);
      break;
    }
    printf("imem[%4d] = 0x%.2x\n", i, imem[i]);
  }
  printf("\n");
}

void dmemdump(int start, int end) {
  int i;
  for (i = start; i <= end; i++) {
    if (i < 0 || i >= IMEMSIZE) {
      printf("Bad location: %d\n", i);
      break;
    }
    printf("dmem[%4d] = 0x%.2x", i, dmem[i]);
    if (i % 8 == 0) {
      u64 val = bytes_to_u64(&dmem[i]);
      printf("   %10lld (0x%.2llx)", val, val);
    }
    printf("\n");
  }
  printf("\n");
}

/* No write_imem function because we never write to the instruction
   memory as part of processing an instruction. */

int readProgram(FILE *in) {
  u64 addr;
  u64 val;

  fscanf(in, " ( ");

  int num_bytes = 0;
  while(fscanf(in, " ( %lld . %lld ) ", &addr, &val) == 2) {
    if (addr > IMEMSIZE) return -1;

    imem[addr] = val;
    if (!num_bytes) {
      /* If this is the first instruction we've seen, record that we
	 have seen at least one valid instruction and set the initial
	 predicted PC to the address of the instruction. */
      f.predPC = addr;
    }
    ++num_bytes;
  }

  return num_bytes;
}

int readData(FILE *in) {
  u64 addr;
  u64 val;

  fscanf(in, " ( ");

  int num_bytes = 0;
  while(fscanf(in, " ( %lld . %lld ) ", &addr, &val) == 2) {
    if (addr > DMEMSIZE) return -1;
    dmem[addr] = val;
    ++num_bytes;
  }

  return num_bytes;
}

/* Initialize pipeline state */
void init() {
  /* f.predPC should be set separately */

  d.stat = 1;
  d.icode = NOP;
  d.rA = RNONE;
  d.rB = RNONE;

  e.stat = 1;
  e.icode = NOP;
  e.dstE = RNONE;
  e.dstM = RNONE;
  e.srcA = RNONE;
  e.srcB = RNONE;

  m.stat = 1;
  m.icode = NOP;
  m.Cnd = 1;
  m.dstE = RNONE;
  m.dstM = RNONE;

  w.stat = 1;
  w.icode = NOP;
  w.dstE = RNONE;
  w.dstM = RNONE;

  stat = AOK;

  cycle_count = 0;
}

void help() {
  printf("List of commands:\n");
  printf("\tcyc <n>                run the simulator for <n> cycles\n");
  printf("\tgo                     run the simulator until halt\n");
  printf("\treg                    display the contents of the register file\n");
  printf("\timem <start> [<end>]   display the contents of instruction memory\n");
  printf("\tdmem <start> [<end>]   display the contents of data memory\n");
  printf("\tpipe                   display the contents of the pipeline latches\n");
  printf("\tb <pc>                 add a breakpoint at particular PC value\n");
  printf("\tsb                     show all breakpoints added\n");
  printf("\tdb                     delete all breakpoints\n");
  printf("\thelp                   get a list of commands\n");
  printf("\tquit                   quit the simulator\n");
}

int shell() {
  char cmd[30];
  printf("> ");

  fgets(cmd, 30, stdin);
  if (feof(stdin)) return 0;

  if (strncmp(cmd, "quit", 4) == 0)
    return 0;
  else if (strncmp(cmd, "help", 4) == 0)
    help();
  else if (strncmp(cmd, "cyc", 3) == 0) {
    int cycles;
    if (sscanf(cmd, "%*s %d", &cycles) == 1) {
      cycles = cycles > 0 ? cycles : 0;
      printf("Simulating for %d cycle(s)...\n", cycles);
      while (cycles-- && stat == AOK) cycle();
      if (stat != AOK) printf("Simulator halted.\n");
    } else {
      printf("Simulating for 1 cycle(s)...\n");
      if (stat == AOK) cycle();
      if (stat != AOK) printf("Simulator halted.\n");
    }
  }
  else if (strncmp(cmd, "go", 2) == 0) {
    int bp = 0;
    while ((stat == AOK) && !bp) bp = cycle();
    if (bp) printf("Hit breakpoint\n");
    else printf("Simulator halted.\n");
  }
  else if (strncmp(cmd, "reg", 3) == 0)
    rgfdump();
  else if (strncmp(cmd, "dmem", 4) == 0) {
    int start, end;
    int num_scanned = sscanf(cmd, "%*s %d %d", &start, &end);
    if (num_scanned == 2) {
      dmemdump(start, end);
    } else if (num_scanned == 1) {
      dmemdump(start, start);
    } else {
      printf("invalid format for dmem; see help for details\n");
    }
  }
  else if (strncmp(cmd, "imem", 4) == 0) {
    int start, end;
    int num_scanned = sscanf(cmd, "%*s %d %d", &start, &end);
    if (num_scanned == 2) {
      imemdump(start, end);
    } else if (num_scanned == 1) {
      imemdump(start, start);
    } else {
      printf("invalid format for imem; see help for details\n");
    }
  }
  else if (strncmp(cmd, "pipe", 4) == 0)
    pipedump();
  else if (strncmp(cmd, "\n", 1) == 0) return 1;
  else if (strncmp(cmd, "sb", 1) == 0) show_breakpoints();
  else if (strncmp(cmd, "db", 1) == 0) {
    printf("Deleting all breakpoints.\n");
    delete_breakpoints();
  }
  else if (strncmp(cmd, "b", 1) == 0) {
    u64 bp;
    if (sscanf(cmd, "%*s %lld\n", &bp) == 1) {
      if (add_breakpoint(bp)) printf("Adding breakpoint at %lld\n", bp);
      else printf("Could not add breakpoint at %lld\n", bp);
    } else {
      printf("invalid format for bp; see help for details\n");
    }
  }
  else printf("Unrecognized command: %s", cmd);

  return 1;
}

int main(int argc, char *argv[]) {
  if (argc < 2) {
    printf("Usage: ./sim <program> [<data>]\n");
    exit(0);
  }

  /* Read first argument as the program */
  FILE *prog;
  if (!(prog = fopen(argv[1], "r"))) {
    printf("File not found: %s\n", argv[1]);
    exit(0);
  }

  int progBytes = readProgram(prog);
  if (progBytes == -1) {
    printf("Bad address in program\n");
    exit(0);
  } else if (progBytes == 0) {
    printf("No valid address found in program\n");
    exit(0);
  } else {
    printf("Read %d bytes into instruction memory.\n", progBytes);
  }
  fclose(prog);

  /* If there is a second argument, read it as the data */
  if (argc > 2) {
    FILE *data;
    if (!(data = fopen(argv[2], "r"))) {
      printf("File not found: %s\n", argv[2]);
      exit(0);
    }
    int dataBytes = readData(data);
    if (dataBytes == -1) {
      printf("Bad address in data\n");
      exit(0);
    } else if (dataBytes == 0) {
      printf("Warning: no valid addresses found in data file\n");
    } else {
      printf("Read %d bytes into data memory\n", dataBytes);
    }
    fclose(data);
  }

  /* initialize pipeline state */
  init();

  /* shell */
  printf("Type \"help\" for a list of commands\n");
  while(shell());

  printf("Bye!\n");
}

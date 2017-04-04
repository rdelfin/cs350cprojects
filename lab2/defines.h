//
// Created by rdelfin on 4/2/17.
//

#ifndef LAB2_DEFINES_H
#define LAB2_DEFINES_H

// Computer is having issues picking up uint8_t and uint64_t
typedef unsigned long uint64_t;
typedef unsigned char uint8_t;

typedef uint64_t y86addr_t;

// REGISTER DEFINES
#define NUM_REGISTERS 15
#define RAX      0
#define RCX      1
#define RDX      2
#define RBX      3
#define RSP      4
#define RBP      5
#define RSI      6
#define RDI      7
#define R8       8
#define R9       9
#define R10      10
#define R11      11
#define R12      12
#define R13      13
#define R14      14
#define REG_NONE 15

// INSTRUCTION DEFINES
#define HALT   0x00
#define NOP    0x10
#define RRMOVQ 0x20
#define CMOVLE 0x21
#define CMOVL  0x22
#define CMOVE  0x23
#define CMOVNE 0x24
#define CMOVGE 0x25
#define CMOVG  0x26
#define IRMOVQ 0x30
#define RMMOVQ 0x40
#define MRMOVQ 0x50
#define ADDQ   0x60
#define SUBQ   0x61
#define ANDQ   0x62
#define XORQ   0x63
#define JMP    0x70
#define JLE    0x71
#define JL     0x72
#define JE     0x73
#define JNE    0x74
#define JGE    0x75
#define JG     0x76
#define CALL   0x80
#define RET    0x90
#define PUSHQ  0xA0
#define POPQ   0xB0
#define IADDQ  0xC0
#define NOOP   0xF0


// Exception codes
#define STAT_AOK 1   // Normal operation
#define STAT_HLT 2   // Halt instruction encountered
#define STAT_ADR 3   // Invalid address encountered
#define STAT_INS 4   // Invalid instruction encountered

// Condition code indices
#define OF_IDX 0
#define ZF_IDX 1
#define SF_IDX 2

// MEMORY DEFINES
#define MAX_MEM (1 << 24)

#endif //LAB2_DEFINES_H

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


// MEMORY DEFINES
#define MAX_MEM (1 << 24)

#endif //LAB2_DEFINES_H

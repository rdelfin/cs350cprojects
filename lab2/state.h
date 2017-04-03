//
// Created by rdelfin on 4/2/17.
//

#ifndef LAB2_STATE_H
#define LAB2_STATE_H

#include "array.h"


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


typedef struct {
    array_t* memory;
    y86addr_t registers[NUM_REGISTERS];
    uint8_t cc[3];
    y86addr_t pc;
    y86addr_t stat;

} state_t;

state_t* state_init();

int state_write_memory(state_t* state, y86addr_t addr, uint8_t value);
int state_read_memory(state_t* state, y86addr_t addr, uint8_t* value);

int state_write_reg(state_t* state, uint8_t reg, y86addr_t value);
int state_read_reg(state_t* state, uint8_t reg, y86addr_t* value);

void state_incr_pc(state_t* state, int steps);
void state_set_pc(state_t* state, int steps);

void state_set_stat(state_t* state, y86addr_t value);
void state_get_stat(state_t* state, y86addr_t* value);

void state_set_cc(state_t* state, uint8_t of, uint8_t zf, uint8_t sf);
void state_get_cc(state_t* state, uint8_t* of, uint8_t* zf, uint8_t* sf);

state_t* state_destroy(state_t* state);

#endif

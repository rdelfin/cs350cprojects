//
// Created by rdelfin on 4/2/17.
//

#ifndef LAB2_STATE_H
#define LAB2_STATE_H

#include <stdio.h>
#include "array.h"
#include "defines.h"

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

void state_print(state_t* state, FILE* f);

state_t* state_destroy(state_t* state);

#endif

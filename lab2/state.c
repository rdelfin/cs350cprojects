//
// Created by rdelfin on 4/2/17.
//

#include "state.h"

#include "array.h"
#include <stdlib.h>


state_t* state_init() {
    state_t* result = malloc(sizeof(state_t));
    result->memory = array_init();
    result->cc[0] = result->cc[1] = result->cc[2] = 0;
    for(int i = 0; i < NUM_REGISTERS; i++, result->registers[i] = 0);
    result->pc = 0;
    result->stat = 0;
}

int state_write_memory(state_t* state, y86addr_t addr, uint8_t value) {
    if(addr >= MAX_MEM)
        return -2;
    return array_set(state->memory, addr, value);
}

int state_read_memory(state_t* state, y86addr_t addr, uint8_t* value) {
    return array_get(state->memory, addr, value);
}

int state_write_reg(state_t* state, uint8_t reg, y86addr_t value) {
    if(reg >= REG_NONE)
        return -1;

    state->registers[reg] = value;
}

int state_read_reg(state_t* state, uint8_t reg, y86addr_t* value) {
    if(reg >= REG_NONE)
        return -1;

    *value = state->registers[reg];
}

void state_incr_pc(state_t* state, int steps) {
    state->pc++;
}

void state_set_pc(state_t* state, int steps) {
    state->pc = steps;

}

void state_set_stat(state_t* state, y86addr_t value) {
    state->stat = value;
}

void state_get_stat(state_t* state, y86addr_t* value) {
    state->stat = value;
}

void state_set_cc(state_t* state, uint8_t of, uint8_t zf, uint8_t sf) {
    state->cc[0] = of ? 1 : 0;
    state->cc[1] = zf ? 1 : 0;
    state->cc[2] = sf ? 1 : 0;
}

void state_get_cc(state_t* state, uint8_t* of, uint8_t* zf, uint8_t* sf) {
    *of = state->cc[0];
    *zf = state->cc[1];
    *sf = state->cc[2];
}

state_t* state_destroy(state_t* state) {
    array_destroy(state->memory);
    free(state);
}


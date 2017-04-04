//
// Created by rdelfin on 4/2/17.
//

#include "state.h"

#include "array.h"
#include <stdlib.h>


state_t* state_init() {
    state_t* result = malloc(sizeof(state_t));
    result->memory = array_init(0);
    result->cc[0] = result->cc[1] = result->cc[2] = 0;
    for(int i = 0; i < NUM_REGISTERS; i++, result->registers[i] = 0);
    result->pc = 0;
    result->stat = STAT_AOK;

    return result;
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

    return 0;
}

int state_read_reg(state_t* state, uint8_t reg, y86addr_t* value) {
    if(reg >= REG_NONE)
        return -1;

    *value = state->registers[reg];

    return 0;
}


int state_write_memory_word(state_t* state, y86addr_t addr, y86addr_t value) {
    uint8_t* val_ptr = (uint8_t*)&value;

    for(int i = 0; i < 8; i++)
        if(state_write_memory(state, addr + i, *(val_ptr + i)))
            return -1;
}

int state_read_memory_word(state_t* state, y86addr_t addr, y86addr_t* value) {
    uint8_t* val_ptr = (uint8_t*)value;

    for(int i = 0; i < 8; i++)
        if(state_read_memory(state, addr + i, val_ptr + i))
            return -1;
}


void state_incr_pc(state_t* state, int steps) {
    state->pc+= steps;
}

void state_set_pc(state_t* state, y86addr_t val) {
    state->pc = val;

}


void state_get_pc(state_t* state, y86addr_t* val) {
    *val = state->pc;
}

int state_read_instruction_byte(state_t* state, uint8_t* value) {
    uint8_t byte;
    if(array_get(state->memory, state->pc, &byte))
        return -1;

    *value = byte;
    state_incr_pc(state, 1);
    return 0;
}

int state_read_instruction_word(state_t* state, y86addr_t* value) {
    uint8_t bytes[8];
    for(int i = 0; i < 8; i++) {
        if(state_read_instruction_byte(state, &bytes[i]))
            return -1;
        state_incr_pc(state, 1);
    }

    return 0;
}

void state_set_stat(state_t* state, y86addr_t value) {
    state->stat = value;
}

void state_get_stat(state_t* state, y86addr_t* value) {
    *value = state->stat;
}

void state_set_cc(state_t* state, uint8_t of, uint8_t zf, uint8_t sf) {
    state->cc[OF_IDX] = (uint8_t)(of ? 1 : 0);
    state->cc[ZF_IDX] = (uint8_t)(zf ? 1 : 0);
    state->cc[SF_IDX] = (uint8_t)(sf ? 1 : 0);
}

void state_get_cc(state_t* state, uint8_t* of, uint8_t* zf, uint8_t* sf) {
    *of = state->cc[OF_IDX];
    *zf = state->cc[ZF_IDX];
    *sf = state->cc[SF_IDX];
}

void state_print(state_t* state, FILE* f) {
    fprintf(f, "Registers\n====================================================\n");
    fprintf(f, "%%rax: 0x%016lx\t%%rcx: 0x%016lx\n", state->registers[RAX], state->registers[RCX]);
    fprintf(f, "%%rdx: 0x%016lx\t%%rbx: 0x%016lx\n", state->registers[RDX], state->registers[RBX]);
    fprintf(f, "%%rsp: 0x%016lx\t%%rbp: 0x%016lx\n", state->registers[RSP], state->registers[RBP]);
    fprintf(f, "%%rsi: 0x%016lx\t%%rdi: 0x%016lx\n", state->registers[RSI], state->registers[RDI]);
    fprintf(f, "%%r8:  0x%016lx\t%%r9:  0x%016lx\n", state->registers[R8], state->registers[R9]);
    fprintf(f, "%%r10: 0x%016lx\t%%r11: 0x%016lx\n", state->registers[R10], state->registers[R11]);
    fprintf(f, "%%r12: 0x%016lx\t%%r13: 0x%016lx\n", state->registers[R12], state->registers[R13]);
    fprintf(f, "%%r14: 0x%016lx\n", state->registers[R14]);

    fprintf(f, "\nProgram Counter: 0x%016lx\n", state->pc);
    fprintf(f, "Status Code: 0x%016lx\n", state->stat);
    fprintf(f, "Condition Code:\n");
    fprintf(f, "\tOF: %d\tZF: %d\tSF: %d\n", state->cc[OF_IDX], state->cc[ZF_IDX], state->cc[SF_IDX]);


    y86addr_t memlen = array_size(state->memory);
    fprintf(f, "\nMemory (%lu bytes)\n==============================================\n", memlen);
    for(y86addr_t i = 0; i < memlen; i++) {
        uint8_t val;
        state_read_memory(state, i, &val);
        fprintf(f, " 0x%08lx: 0x%02x\n", i, (unsigned)val);
    }
}

state_t* state_destroy(state_t* state) {
    array_destroy(state->memory);
    free(state);
}


//
// Created by rdelfin on 4/3/17.
//

#ifndef LAB2_INSTRUCTION_H
#define LAB2_INSTRUCTION_H

#include "state.h"

typedef struct {
    uint8_t code;
    uint8_t r1;
    uint8_t r2;
    uint64_t val;
} instruction_t;

// Simulates the next instruction
int execute_next_instruction(state_t* state);

// Converts the instruction at the current program counter into an instruction struct. Also advances PC.
int instruction_to_struct(state_t* state, instruction_t* instr);

// These functions alter the state according to the y86_64 spec and using the instruction struct provided
int execute_halt(state_t* state, instruction_t instr);
int execute_nop(state_t* state, instruction_t instr);
int execute_rrmovq(state_t* state, instruction_t instr);
int execute_rmmovq(state_t* state, instruction_t instr);
int execute_mrmovq(state_t* state, instruction_t instr);
int execute_opq(state_t* state, instruction_t instr);
int execute_jmp(state_t* state, instruction_t instr);
int execute_call(state_t* state, instruction_t instr);
int execute_ret(state_t* state, instruction_t instr);
int execute_pushq(state_t* state, instruction_t instr);
int execute_popq(state_t* state, instruction_t instr);
int execute_iaddq(state_t* state, instruction_t instr);

uint8_t condition_eval(state_t* state, uint8_t condition);

#endif //LAB2_INSTRUCTION_H

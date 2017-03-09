#ifndef _INSTRUCTIONS_H_
#define _INSTRUCTIONS_H_

#include <stdint.h>

typedef struct {
    uint8_t len;
    uint8_t instr_code;
    uint8_t registers;
    uint64_t value;
} instruction_t;

uint8_t* instruction_write_bytes(uint8_t* data, instruction_t* instr);

#endif

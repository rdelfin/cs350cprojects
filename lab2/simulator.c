//
// Created by rdelfin on 4/3/17.
//

#include <stdint.h>
#include "simulator.h"
#include "state.h"

int execute_next_instruction(state_t* state) {
    instruction_t instr;
    int ins_result = instruction_to_struct(state, &instr);
    if(ins_result == -2) {
        state_set_stat(state, STAT_INS);
        return 0;
    } else if(ins_result)
        return ins_result;

    switch (instr.code) {
        case HALT:
            return execute_halt(state, instr);

        case NOP:
        case NOOP:
            return execute_nop(state, instr);

        case RRMOVQ:
        case CMOVLE:
        case CMOVL:
        case CMOVE:
        case CMOVNE:
        case CMOVGE:
        case CMOVG:
            return execute_rrmovq(state, instr);

        case IRMOVQ:
            return execute_irmovq(state, instr);

        case RMMOVQ:
            return execute_rmmovq(state, instr);

        case MRMOVQ:
            return execute_mrmovq(state, instr);

        case ADDQ:
        case SUBQ:
        case ANDQ:
        case XORQ:
            return execute_opq(state, instr);

        case JMP:
        case JLE:
        case JL:
        case JE:
        case JNE:
        case JGE:
        case JG:
            return execute_jmp(state, instr);

        case CALL:
            return execute_call(state, instr);

        case RET:
            return execute_ret(state, instr);

        case PUSHQ:
            return execute_pushq(state, instr);

        case POPQ:
            return execute_popq(state, instr);

        case IADDQ:
            return execute_iaddq(state, instr);

        default:
            state_set_stat(state, STAT_INS);
            return 0;
    }
}

int instruction_to_struct(state_t* state, instruction_t* instr) {
    instr->r1 = 0;
    instr->r2 = 0;
    instr->val = 0;

    if(state_read_instruction_byte(state, &(instr->code)))
        return -1;

    y86addr_t val;
    uint8_t reg;

    switch (instr->code) {
        case HALT:
        case NOP:
        case RET:
        case NOOP:
            // These instructions only contain instruction code
            break;

        case RRMOVQ:
        case CMOVLE:
        case CMOVL:
        case CMOVE:
        case CMOVNE:
        case CMOVGE:
        case CMOVG:
        case ADDQ:
        case SUBQ:
        case ANDQ:
        case XORQ:
        case PUSHQ:
        case POPQ:
            // These instructions need to registers read only
            if(state_read_instruction_byte(state, &reg))
                return -2;
            instr->r1 = (reg >> 4) & (uint8_t)0x0F;
            instr->r2 = reg & (uint8_t)0x0F;
            break;


        case CALL:
        case JMP:
        case JLE:
        case JL:
        case JE:
        case JNE:
        case JGE:
        case JG:
            // These instructions only need to read in the value
            if(state_read_instruction_word(state, &val))
                return -3;
            instr->val = val;
            break;


        case IADDQ:
        case IRMOVQ:
        case RMMOVQ:
        case MRMOVQ:
            // These instruction read in both a value and registers
            if(state_read_instruction_byte(state, &reg))
                return -2;
            if(state_read_instruction_word(state, &val))
                return -3;
            instr->r1 = (reg >> 4) & (uint8_t)0x0F;
            instr->r2 = reg & (uint8_t)0x0F;
            instr->val = val;
            break;

        default:
            return -2;

    }

    return 0;
}


int execute_halt(state_t* state, instruction_t instr) {
    state_set_stat(state, STAT_HLT);
    return 0;
}

int execute_nop(state_t* state, instruction_t instr) {
    return 0;
}

int execute_rrmovq(state_t* state, instruction_t instr) {
    if(instr.r1 ==  REG_NONE || instr.r2 == REG_NONE) {
        state_set_stat(state, STAT_INS);
        return 0;
    }

    uint8_t condition = instr.code & (uint8_t)0x0F;
    if(condition > 6) {
        state_set_stat(state, STAT_INS);
        return 0;
    }

    uint8_t cond_val = condition_eval(state, condition);
    if(cond_val) {
        y86addr_t r1val;
        if(state_read_reg(state, instr.r1, &r1val))
            return -1;

        if(state_write_reg(state, instr.r2, r1val))
            return -1;
    }

    return 0;
}

int execute_irmovq(state_t* state, instruction_t instr) {
    if(instr.r1 != REG_NONE || instr.r2 == REG_NONE) {
        state_set_stat(state, STAT_INS);
        return 0;
    }

    if(state_write_reg(state, instr.r2, instr.val))
        return -1;

    return 0;
}

int execute_rmmovq(state_t* state, instruction_t instr) {
    if(instr.r1 == REG_NONE || instr.r2 == REG_NONE) {
        state_set_stat(state, STAT_INS);
        return 0;
    }

    y86addr_t r1val, r2val;
    if(state_read_reg(state, instr.r1, &r1val))
        return -1;
    if(state_read_reg(state, instr.r2, &r2val))
        return -1;
    if(state_write_memory_word(state, r2val, r1val))
        return -1;

    return 0;
}

int execute_mrmovq(state_t* state, instruction_t instr) {
    if(instr.r1 == REG_NONE || instr.r2 == REG_NONE) {
        state_set_stat(state, STAT_INS);
        return 0;
    }

    y86addr_t r2val;
    y86addr_t r2_mem_val;
    if(state_read_reg(state, instr.r2, &r2val))
        return -1;
    if(state_read_memory_word(state, r2val, &r2_mem_val))
        return -1;
    if(state_write_reg(state, instr.r1, r2_mem_val))
        return -1;
    return 0;
}

int execute_opq(state_t* state, instruction_t instr) {
    if(instr.r1 == REG_NONE || instr.r2 == REG_NONE) {
        state_set_stat(state, STAT_INS);
        return 0;
    }

    y86addr_t r1val, r2val, result = 0;
    uint8_t zf, sf, of;
    if(state_read_reg(state, instr.r1, &r1val))
        return -1;
    if(state_read_reg(state, instr.r2, &r2val))
        return -1;

    int64_t r1val_s = r1val, r2val_s = r2val, result_s;

    switch (instr.code & 0x0F) {
        case 0x00:   // Addq

            result_s = r1val_s + r2val_s;
            of = (uint8_t) ((r1val_s < 0 == r2val_s < 0) && (result_s < 0 != r1val_s < 0));
            result = (y86addr_t)result_s;
            break;

        case 0x01:   // subq
            result_s = r2val_s - r1val_s;
            r1val_s = -r1val_s;
            of = (uint8_t) ((r1val_s < 0 == r2val_s < 0) && (result_s < 0 != r1val_s < 0));
            result = (y86addr_t)result_s;
            break;

        case 0x02:   // andq
            result = r2val & r1val;
            of = 0;
            break;

        case 0x03:   // xorq
            result = r2val ^ r1val;
            of = 0;
            break;

        default:
            state_set_stat(state, STAT_INS);
            return 0;
    }

    zf = (uint8_t) (result == 0);
    sf = (uint8_t) (((int64_t)result) < 0);

    state_write_reg(state, instr.r2, result);
    state_set_cc(state, of, zf, sf);

    return 0;
}

int execute_jmp(state_t* state, instruction_t instr) {
    uint8_t condition = instr.code & (uint8_t)0x0F;
    if(condition > 6) {
        state_set_stat(state, STAT_INS);
        return 0;
    }

    uint8_t cond_val = condition_eval(state, condition);
    if(cond_val) {
        state_set_pc(state, instr.val);
    }

    return 0;
}

int execute_call(state_t* state, instruction_t instr) {
    y86addr_t rspval, pcval;

    state_get_pc(state, &pcval);
    if(state_read_reg(state, RSP, &rspval))
        return -1;
    if(state_write_memory_word(state, rspval - 8, pcval))
        return -1;
    if(state_write_reg(state, RSP, rspval - 8))
        return -1;
    state_set_pc(state, instr.val);

    return 0;
}

int execute_ret(state_t* state, instruction_t instr) {
    y86addr_t rspval, retval;

    if(state_read_reg(state, RSP, &rspval))
        return -1;
    if(state_read_memory_word(state, rspval, &retval))
        return -1;
    if(state_write_reg(state, RSP, rspval + 8))
        return -1;
    state_set_pc(state, retval);

    return 0;
}

int execute_pushq(state_t* state, instruction_t instr) {
    if(instr.r1 == REG_NONE || instr.r2 != REG_NONE) {
        state_set_stat(state, STAT_INS);
        return 0;
    }

    y86addr_t r1val, rspval;

    if(state_read_reg(state, instr.r1, &r1val))
        return -1;
    if(state_read_reg(state, RSP, &rspval))
        return -1;
    if(state_write_memory_word(state, rspval - 8, r1val))
        return  -1;
    if(state_write_reg(state, RSP, rspval - 8))
        return -1;

    return 0;
}

int execute_popq(state_t* state, instruction_t instr) {
    if(instr.r1 == REG_NONE || instr.r2 != REG_NONE) {
        state_set_stat(state, STAT_INS);
        return 0;
    }

    y86addr_t rspval, memval;
    if(state_read_reg(state, RSP, &rspval))
        return -1;
    if(state_read_memory_word(state, rspval, &memval))
        return -1;
    if(state_write_reg(state, RSP, rspval + 8))
        return -1;
    if(state_write_reg(state, instr.r1, memval))
        return -1;

    return 0;
}

int execute_iaddq(state_t* state, instruction_t instr) {
    if(instr.r1 != REG_NONE || instr.r2 == REG_NONE) {
        state_set_stat(state, STAT_INS);
        return 0;
    }

    y86addr_t r2val, result;
    uint8_t zf, sf, of;
    int64_t r2val_s, result_s, value_s;
    if(state_read_reg(state, instr.r2, &r2val))
        return -1;

    r2val_s = r2val;
    value_s = instr.val;
    result_s = r2val_s + value_s;
    result = (y86addr_t) result_s;

    of = (uint8_t) ((value_s < 0 == r2val_s < 0) && (result_s < 0 != value_s < 0));
    zf = (uint8_t) (result == 0);
    sf = (uint8_t) (((int64_t)result) < 0);
    state_set_cc(state, of, zf, sf);

    if(state_write_reg(state, instr.r2, result))
        return -1;

    return 0;
}


/**
 * Computes a logical xor over a and b (as opposed to the bitwise a ^ b operator)
 */
static inline uint8_t xor(uint8_t a, uint8_t b) {
    return (!a && b) || (a && !b);
}

uint8_t condition_eval(state_t* state, uint8_t condition) {
    uint8_t sf, zf, of;
    state_get_cc(state, &of, &zf, &sf);

    switch (condition) {
        case 0:   // No condition
            return 1;
        case 1:   // LE
            return xor(sf, of) || zf;
        case 2:   // L
            return xor(sf, of);
        case 3:   // E
            return !!zf;
        case 4:   // NE
            return !zf;
        case 5:   // GE
            return !xor(sf, of);
        case 6:   // G
            return !xor(zf, of) && !zf;
        default:
            return 0;
    }
}

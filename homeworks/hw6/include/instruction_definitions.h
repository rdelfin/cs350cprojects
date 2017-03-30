//
// Created by rdelfin on 3/28/17.
//

#ifndef HW6_INSTRUCTION_DEFINITIONS_H
#define HW6_INSTRUCTION_DEFINITIONS_H

#include <unordered_map>

#define HALT   0x00
#define NOP    0x10
#define RRMOVL 0x20
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
#define LEAVE  0xD0
#define NOOP   0xF0

class instruction_maps {
public:
    static std::unordered_map<std::string, uint8_t> sizes() {
        return {
                {"halt",   1},
                {"nop",    1},
                {"noop",   1},
                {"rrmovq", 2},
                {"cmovle", 2},
                {"cmovl",  2},
                {"cmove",  2},
                {"cmovne", 2},
                {"cmovge", 2},
                {"cmovg",  2},
                {"irmovq", 10},
                {"rmmovq", 10},
                {"mrmovq", 10},
                {"addq",   2},
                {"subq",   2},
                {"andq",   2},
                {"xorq",   2},
                {"jmp",    9},
                {"jle",    9},
                {"jl",     9},
                {"je",     9},
                {"jne",    9},
                {"jge",    9},
                {"jg",     9},
                {"call",   9},
                {"ret",    1},
                {"pushq",  2},
                {"popq",   2},
                {"iaddq",  10},
                {"leave",  1},
                {"noop",   1}
        };
    }

    static std::unordered_map<std::string, uint8_t> registers() {
        return {
                {"%rax", 0},
                {"%rcx", 1},
                {"%rdx", 2},
                {"%rbx", 3},
                {"%rsi", 4},
                {"%rdi", 5},
                {"%rsp", 6},
                {"%rbp", 7},
                {"%r8",  8},
                {"%r9",  9},
                {"%r10", 10},
                {"%r11", 11},
                {"%r12", 12},
                {"%r13", 13},
                {"%r14", 14}
        };
    }

    static std::unordered_map<std::string, uint8_t> codes() {
        return {
                { "halt", HALT },
                { "nop", NOP },
                { "rrmovq", RRMOVL },
                { "cmovle", CMOVLE },
                { "cmovl", CMOVL },
                { "cmove", CMOVE },
                { "cmovne", CMOVNE },
                { "cmovge", CMOVGE },
                { "cmovg", CMOVG },
                { "irmovq", IRMOVQ },
                { "rmmovq", RMMOVQ },
                { "mrmovq", MRMOVQ },
                { "addq", ADDQ },
                { "subq", SUBQ },
                { "andq", ANDQ },
                { "xorq", XORQ },
                { "jmp", JMP },
                { "jle", JLE },
                { "jl", JL },
                { "je", JE },
                { "jne", JNE },
                { "jge", JGE },
                { "jg", JG },
                { "call", CALL },
                { "ret", RET },
                { "pushq", PUSHQ },
                { "popq", POPQ },
                { "iaddq", IADDQ },
                { "leave", LEAVE },
                { "noop", NOOP }
        };
    }
};

#endif //HW6_INSTRUCTION_DEFINITIONS_H

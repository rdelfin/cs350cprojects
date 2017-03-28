#include <instruction.h>
#include <instruction_definitions.h>

#include <cstdint>
#include <string>
#include <sstream>
#include <iostream>
#include <map>
#include <algorithm>



inline bool is_whitespace(char c) {
    return c == ' ' || c == '\t' || c == '\n' || c == '\r';
}

inline bool isInteger(const std::string & s)
{
    if(s.empty() || ((!isdigit(s[0])) && (s[0] != '-') && (s[0] != '+'))) return false ;

    char * p ;
    strtol(s.c_str(), &p, 10) ;

    return (*p == 0) ;
}

instruction::instruction() {
}

instruction::instruction(std::string code, const std::map<std::string, y86addr_t> labelmap) {
    this->r1 = 0;
    this->r2 = 0;
    this->value = 0;

    std::stringstream codestream(code);
    std::string instr_string;
    int c = codestream.get();
    while(c != EOF && is_whitespace((char)c)) {
    }

    if(codestream.get() != '(') {
        std::cerr << "There was an error. Parenthesis expected when parsing" << std::endl;
        throw "parenthesis not found when parsing";
    }

    codestream >> instr_string;

    this->instr_code = instr_string_to_code(instr_string);
    this->len = instr_string_to_size(instr_string);

    switch (instr_code) {
        case HALT:
            parse_as_halt(codestream);
            break;
        case NOP:
            parse_as_nop(codestream);
            break;
        case RRMOVL:
        case CMOVLE:
        case CMOVL:
        case CMOVE:
        case CMOVNE:
        case CMOVGE:
        case CMOVG:
            parse_as_rrmovl(codestream);
            break;
        case RMMOVQ:
            parse_as_rmmovl(codestream);
            break;
        case MRMOVQ:
            parse_as_mrmovl(codestream);
            break;
        case ADDQ:
        case SUBQ:
        case ANDQ:
        case XORQ:
            parse_as_op(codestream);
            break;
        case JMP:
        case JLE:
        case JL:
        case JE:
        case JNE:
        case JGE:
        case JG:
            parse_as_jmp(codestream);
            break;
        case CALL:
            parse_as_call(codestream);
            break;
        case RET:
            parse_as_ret(codestream);
            break;
        case PUSHQ:
            parse_as_pushl(codestream);
            break;
        case POPQ:
            parse_as_popl(codestream);
            break;
    }

}


uint8_t instruction::reg_string_to_code(std::string str) {
    long int reg = strtol(str.c_str(), nullptr, 10);
    if(isInteger(str) && reg >= 0 && reg <= 14) {
        return (uint8_t)reg;
    }


    std::transform(str.begin(), str.end(), str.begin(), ::tolower);

    if(instruction_maps::registers().count(str) == 0) {
        throw "Unexpected instruction";
    }

    return instruction_maps::registers()[str];
}

uint8_t instruction::instr_string_to_size(std::string str) {

    std::transform(str.begin(), str.end(), str.begin(), ::tolower);

    if(instruction_maps::sizes().count(str) == 0) {
        throw "Unexpected instruction";
    }

    return instruction_maps::sizes()[str];
}

uint8_t instruction::instr_string_to_code(std::string str) {

    std::transform(str.begin(), str.end(), str.begin(), ::tolower);

    if(instruction_maps::codes().count(str) == 0) {
        throw "Unexpected instruction";
    } return instruction_maps::codes()[str];
}

/* Individual parsers for each instruction */
void instruction::parse_as_halt(std::stringstream&) {
    // Do nothing. Is already parsed
}


void instruction::parse_as_nop(std::stringstream&) {
    // Do nothing. Is already parsed
}



void instruction::parse_as_rrmovl(std::stringstream& stream) {
    // Parse two registers
    std::string reg1, reg2;
    stream >> reg1 >> reg2;

    if(reg1[reg1.size() - 1] != ',' || reg2[reg2.size() - 1] != ')') {
        throw "Invalid register token";
    }

    this->r1 = reg_string_to_code(reg1.substr(0, reg1.size() - 1));
    this->r2 = reg_string_to_code(reg2.substr(0, reg2.size() - 1));
}


void instruction::parse_as_rmmovl(std::stringstream& stream) {
    // Parse two registers
    std::string reg1, reg2, val;
    stream >> val >> reg1 >> reg2;

    if(reg1[reg1.size() - 1] != ',' || reg2[reg2.size() - 1] != ')') {
        throw "Invalid register token";
    }

    this->r1 = reg_string_to_code(reg1.substr(1, reg1.size() - 1));
    this->r2 = reg_string_to_code(reg2.substr(0, reg2.size() - 1));

    // Use value somehow
}


void instruction::parse_as_mrmovl(std::stringstream& stream) {
    // Parse two registers
    std::string reg1, reg2, val;
    stream >> reg1 >> val >> reg2;

    if(reg1[reg1.size() - 1] != ',' || reg2[reg2.size() - 1] != ')') {
        throw "Invalid register token";
    }

    this->r1 = reg_string_to_code(reg1.substr(0, reg1.size() - 1));
    this->r2 = reg_string_to_code(reg2.substr(1, reg2.size() - 1));

    // Use value somehow
}


void instruction::parse_as_op(std::stringstream& stream) {
    parse_as_rrmovl(stream);
}


void instruction::parse_as_jmp(std::stringstream& stream) {
    std::string val;
    stream >> val;
}


void instruction::parse_as_call(std::stringstream& stream) {
   parse_as_jmp(stream);
}


void instruction::parse_as_ret(std::stringstream&) {
    // Do nothing
}


void instruction::parse_as_pushl(std::stringstream& stream) {
   std::string reg;
    stream >> reg;

    if(reg[reg.size() - 1] == ')') {
        reg = reg.substr(0, reg.size() - 1);
    }

    this->r1 = reg_string_to_code(reg);
    this->r2 = 0xF;
}


void instruction::parse_as_popl(std::stringstream& stream) {
    parse_as_pushl(stream);
}



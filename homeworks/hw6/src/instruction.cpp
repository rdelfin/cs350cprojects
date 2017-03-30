#include <instruction.h>
#include <instruction_definitions.h>
#include <parsing_functions.h>

#include <string>
#include <sstream>
#include <iostream>
#include <map>
#include <algorithm>

instruction::instruction() {
}

instruction::instruction(node* n, const std::unordered_map<std::string, y86addr_t> labelmap, bool ignore_label_error) {
    this->r1 = 0;
    this->r2 = 0;
    this->value = 0;

    if(n->children.size() == 0)
        throw InvalidInstructionException("Instruction has no contents (empty parenthesis)");
    if(!n->children[0]->isLeaf)
        throw InvalidInstructionException("Instruction name is nested.");

    std::string instr_string = n->children[0]->value;

    this->instr_code = instr_string_to_code(instr_string);
    this->len = instr_string_to_size(instr_string);

    switch (instr_code) {
        case HALT:
            parse_as_halt(n);
            break;
        case NOP:
        case NOOP:
            parse_as_nop(n);
            break;
        case RRMOVL:
        case CMOVLE:
        case CMOVL:
        case CMOVE:
        case CMOVNE:
        case CMOVGE:
        case CMOVG:
            parse_as_rrmovq(n);
            break;
        case RMMOVQ:
            parse_as_rmmovq(n, labelmap, ignore_label_error);
            break;
        case MRMOVQ:
            parse_as_mrmovq(n, labelmap, ignore_label_error);
            break;
        case ADDQ:
        case SUBQ:
        case ANDQ:
        case XORQ:
            parse_as_op(n);
            break;
        case JMP:
        case JLE:
        case JL:
        case JE:
        case JNE:
        case JGE:
        case JG:
            parse_as_jmp(n, labelmap, ignore_label_error);
            break;
        case CALL:
            parse_as_call(n, labelmap, ignore_label_error);
            break;
        case RET:
            parse_as_ret(n);
            break;
        case PUSHQ:
            parse_as_pushq(n);
            break;
        case POPQ:
            parse_as_popq(n);
            break;
        case IRMOVQ:
            parse_as_irmovq(n, labelmap, ignore_label_error);
            break;
        case LEAVE:
            parse_as_leave(n);
        default:
            std::stringstream ss;
            ss << "Invalid instruction! Instruction " << instr_string << " is not defined.";
            throw InvalidInstructionException(ss.str());
    }

}

void instruction::write_to_memory(std::map<y86addr_t, uint8_t> memMap, y86addr_t* addr) {
    uint8_t bytes[10];
    // Special case for call and jmp
    if(instr_code == CALL || instr_code == JMP || instr_code == JLE || instr_code == JL || instr_code == JE ||
       instr_code == JNE  || instr_code == JG) {
        bytes[0] = instr_code;
        // ASSUMPTION: This is a little endian machine
        y86addr_t* val_ptr = (y86addr_t*)(bytes + 1);
        *val_ptr = value;
    } else {
        bytes[0] = instr_code;
        bytes[1] = (r1 << 8) & r2;
        // ASSUMPTION: This is a little endian machine
        y86addr_t* val_ptr = (y86addr_t*)(bytes + 2);
        *val_ptr = value;
    }

    for(size_t i = 0; i < len; i++, *addr++)
        memMap[*addr] = bytes[i];
}


uint8_t instruction::reg_string_to_code(const std::string& str) {
    long int reg = strtol(str.c_str(), nullptr, 10);
    if(isDecimalInteger(str) && reg >= 0 && reg <= 14) {
        return (uint8_t)reg;
    }

    std::string mutable_string = str;
    std::transform(mutable_string.begin(), mutable_string.end(), mutable_string.begin(), ::tolower);

    if(instruction_maps::registers().count(mutable_string) == 0) {
        std::stringstream ss;
        ss << "Unexpected register name " << mutable_string;
        throw InvalidInstructionException(ss.str());
    }

    return instruction_maps::registers()[str];
}

uint8_t instruction::instr_string_to_size(const std::string& val) {
    // Create a mutable copy
    std::string str = val;

    std::transform(str.begin(), str.end(), str.begin(), ::tolower);

    if(instruction_maps::sizes().count(str) == 0) {
        std::stringstream ss;
        ss << "Unexpected instruction name " << str;
        throw InvalidInstructionException(ss.str());
    }

    return instruction_maps::sizes()[str];
}

uint8_t instruction::instr_string_to_code(const std::string& val) {
    // Create a mutable copy
    std::string str = val;

    std::transform(str.begin(), str.end(), str.begin(), ::tolower);

    if(instruction_maps::codes().count(str) == 0) {
        std::stringstream ss;
        ss << "Unexpected instruction name " << str;
        throw InvalidInstructionException(ss.str());
    }

    return instruction_maps::codes()[str];
}

/* Individual parsers for each instruction */
void instruction::parse_as_halt(node* n) {
    if(n->children.size() != 1)
        throw InvalidInstructionException("halt requres exactly 0 arguments");
    // Do nothing. Is already parsed
}


void instruction::parse_as_nop(node* n) {
    if(n->children.size() != 1)
        throw InvalidInstructionException("halt requres exactly 0 arguments");
    // Do nothing. Is already parsed
}



void instruction::parse_as_rrmovq(node* n) {
    // There should be exactly 3 arguments
    if(n->children.size() != 3)
        throw InvalidInstructionException("This instruction requires exactly 2 arguments");
    if(!n->children[1]->isLeaf)
        throw InvalidInstructionException("The first register of this instruction should not have parenthesis");
    if(!n->children[2]->isLeaf)
        throw InvalidInstructionException("The second register of this instruction should not have parenthesis");

    // Parse two registers
    std::string reg1 = n->children[1]->value, reg2 = n->children[2]->value;

    this->r1 = reg_string_to_code(reg1);
    this->r2 = reg_string_to_code(reg2);
}


void instruction::parse_as_rmmovq(node* n, const std::unordered_map<std::string, y86addr_t> labelmap, bool ignore_label_error) {

    if(n->children.size() != 3 && n->children.size() != 4)
        throw InvalidInstructionException("rmmovq requires exactly 2 arguments");

    bool has_offset = n->children.size() == 4;
    int r2_idx = has_offset ? 3 : 2;

    if(!n->children[1]->isLeaf)
        throw InvalidInstructionException("The first register of rmmovq should not have parenthesis");
    if(!((has_offset && n->children[2]->isLeaf) || !has_offset))
        throw InvalidInstructionException("The offset of rmmovq is present and has parenthesis (which it should not have)");
    if(n->children[r2_idx]->isLeaf || n->children[r2_idx]->children.size() != 1 || !n->children[r2_idx]->children[0]->isLeaf)
        throw InvalidInstructionException("The second register of rmmovq should be a register in exactly one set of parenthesis");

    // Parse two registers
    std::string reg1 = n->children[1]->value, reg2 = n->children[r2_idx]->children[0]->value, value = has_offset ? n->children[2]->value : "";

    this->r1 = reg_string_to_code(reg1);
    this->r2 = reg_string_to_code(reg2);
    this->value = has_offset ? immediate_string_to_value(value, labelmap, ignore_label_error) : 0;
}


void instruction::parse_as_mrmovq(node* n, const std::unordered_map<std::string, y86addr_t> labelmap, bool ignore_label_error) {

    if(n->children.size() != 3 && n->children.size() != 4)
        throw InvalidInstructionException("mrmovq requires exactly 2 arguments");

    bool has_offset = n->children.size() == 4;
    int r1_idx = has_offset ? 2 : 1;
    int r2_idx = has_offset ? 3 : 2;

    if(!n->children[r2_idx]->isLeaf)
        throw InvalidInstructionException("The second register of mrmovq should not have parenthesis");
    if(!((has_offset && n->children[1]->isLeaf) || !has_offset))
        throw InvalidInstructionException("The offset of mrmovq is present and has parenthesis (which it should not have)");
    if(n->children[r1_idx]->isLeaf || n->children[r1_idx]->children.size() != 1 || !n->children[r1_idx]->children[0]->isLeaf)
        throw InvalidInstructionException("The first register of mrmovq should be a register in exactly one set of parenthesis");

    // Parse two registers
    std::string reg1 = n->children[r1_idx]->children[0]->value, reg2 = n->children[r2_idx]->value, value = has_offset ? n->children[1]->value : "";

    this->r1 = reg_string_to_code(reg1);
    this->r2 = reg_string_to_code(reg2);
    this->value = has_offset ? immediate_string_to_value(value, labelmap, ignore_label_error) : 0;
}


void instruction::parse_as_op(node* n) {
    parse_as_rrmovq(n);
}


void instruction::parse_as_jmp(node* n, const std::unordered_map<std::string, y86addr_t> labelmap, bool ignore_label_error) {
    if(n->children.size() != 2)
        throw InvalidInstructionException("This instruction must have exactly one argument");

    if(!n->children[1]->isLeaf)
        throw InvalidInstructionException("The immediate value of this instruction should have no parenthesis");

    std::string value = n->children[1]->value;

    this->value = immediate_string_to_value(value, labelmap, ignore_label_error);
}


void instruction::parse_as_call(node* n, const std::unordered_map<std::string, y86addr_t> labelmap, bool ignore_label_error) {
   parse_as_jmp(n, labelmap, ignore_label_error);
}


void instruction::parse_as_ret(node* n) {
    // Do nothing
}


void instruction::parse_as_pushq(node* n) {
    if(n->children.size() != 2)
        throw InvalidInstructionException("This instruction must have exactly one argument");
    if(!n->children[1]->isLeaf)
        throw InvalidInstructionException("The register in this instruction cannot have parenthesis");

    std::string reg = n->children[1]->value;

    this->r1 = reg_string_to_code(reg);
    this->r2 = 0xF;
}


void instruction::parse_as_popq(node* n) {
    parse_as_pushq(n);
}

void instruction::parse_as_leave(node* n) {
    parse_as_halt(n);
}

void instruction::parse_as_irmovq(node* n, const std::unordered_map<std::string, y86addr_t> labelmap, bool ignore_label_error) {
    if(n->children.size() != 3)
        throw InvalidInstructionException("irmovq must have exactly 2 arguments");
    if(!n->children[1]->isLeaf)
        throw InvalidInstructionException("The immediate value of irmovq should have no parenthesis");
    if(!n->children[2]->isLeaf)
        throw InvalidInstructionException("The register of irmovq should have no parenthesis");

    this->r1 = 0x0F;
    this->r2 = reg_string_to_code(n->children[2]->value);
    this->value = immediate_string_to_value(n->children[1]->value, labelmap, ignore_label_error);
}

void instruction::parse_as_iaddq(node* n, const std::unordered_map<std::string, y86addr_t> labelmap, bool ignore_label_error) {
    parse_as_irmovq(n, labelmap, ignore_label_error);
}

y86addr_t instruction::immediate_string_to_value(const std::string& value,
                                                 std::unordered_map<std::string, y86addr_t> labelmap,
                                                 bool ignore_label_error) {
    y86addr_t val = 0;

    // For decimal, parse through strtol() with base 10
    if(isDecimalInteger(value))
        val = (y86addr_t)strtol(value.c_str(), nullptr, 10);

    // For hexadecimal, replace '#' with '0x' and pass through strtol()
    else if(isHexInteger(value)) {
        std::stringstream ss;
        for(const char* p = value.c_str(); *p != '\0'; p++) {
            if(*p == '#')
                ss << "0x";
            else
                ss.put(*p);
        }
        val = (y86addr_t)strtol(ss.str().c_str(), nullptr, 0);
    }

    // Otherwise, search through labels
    else {
        auto it = labelmap.find(value);
        if(it == labelmap.end()) {
            if(!ignore_label_error) {
                std::stringstream ss;
                ss << "Could not find the label " << value;
                   throw InvalidLabelException(ss.str());
            }
        } else {
            val = labelmap[value];
        }
    }

    return val;
}


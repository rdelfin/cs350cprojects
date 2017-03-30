#ifndef _INSTRUCTIONS_H_
#define _INSTRUCTIONS_H_

#include <cstdint>
#include <string>
#include <map>

#include <instruction_definitions.h>
#include <tree.h>


class instruction {
public:
    instruction();
    instruction(node* n, const std::unordered_map<std::string, y86addr_t> labelmap, bool ignore_label_error);

    void write_to_memory(std::map<y86addr_t, uint8_t> memMap, y86addr_t* addr);

    uint8_t len;
    uint8_t instr_code;
    uint8_t r1;
    uint8_t r2;
    uint64_t value;
private:
    void init(uint8_t len, uint8_t instr_code);
    uint8_t instr_string_to_code(const std::string& str);
    uint8_t instr_string_to_size(const std::string& str);

    uint8_t reg_string_to_code(const std::string& str);

    void parse_as_halt(node *);
    void parse_as_nop(node *);
    void parse_as_rrmovq(node *);
    void parse_as_rmmovq(node *, const std::unordered_map<std::string, y86addr_t> labelmap, bool ignore_label_error);
    void parse_as_mrmovq(node *, const std::unordered_map<std::string, y86addr_t> labelmap, bool ignore_label_error);
    void parse_as_op(node *);
    void parse_as_jmp(node *, const std::unordered_map<std::string, y86addr_t> labelmap, bool ignore_label_error);
    void parse_as_call(node *, const std::unordered_map<std::string, y86addr_t> labelmap, bool ignore_label_error);
    void parse_as_ret(node *);
    void parse_as_pushq(node *);
    void parse_as_popq(node *);
    void parse_as_leave(node *);
    void parse_as_irmovq(node *, const std::unordered_map<std::string, y86addr_t> labelmap, bool ignore_label_error);
    void parse_as_iaddq(node*, const std::unordered_map<std::string, y86addr_t> labelmap, bool ignore_label_error);

    y86addr_t immediate_string_to_value(const std::string&,
                                        std::unordered_map<std::string, y86addr_t> labelmap,
                                        bool ignore_label_error);
};

class InvalidInstructionException {
public:
    InvalidInstructionException(const std::string& msg) : message(msg) { }
    std::string message;
};

class InvalidLabelException {
public:
    InvalidLabelException(const std::string& msg) : message(msg) { }
    std::string message;
};

#endif

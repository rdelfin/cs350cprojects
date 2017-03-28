#ifndef _INSTRUCTIONS_H_
#define _INSTRUCTIONS_H_

#include <cstdint>
#include <string>
#include <map>

typedef uint64_t y86addr_t;

class instruction {
public:
    instruction();
    instruction(std::string code, const std::map<std::string, y86addr_t> labelmap);
private:
    void init(uint8_t len, uint8_t instr_code);
    uint8_t instr_string_to_code(std::string str);
    uint8_t instr_string_to_size(std::string str);

    uint8_t reg_string_to_code(std::string str);

    void parse_as_halt(std::stringstream&);
    void parse_as_nop(std::stringstream&);
    void parse_as_rrmovl(std::stringstream&);
    void parse_as_rmmovl(std::stringstream&);
    void parse_as_mrmovl(std::stringstream&);
    void parse_as_op(std::stringstream&);
    void parse_as_jmp(std::stringstream&);
    void parse_as_call(std::stringstream&);
    void parse_as_ret(std::stringstream&);
    void parse_as_pushl(std::stringstream&);
    void parse_as_popl(std::stringstream&);

    std::string read_register(std::stringstream&);
    std::string read_whitespace(std::stringstream&);


    uint8_t len;
    uint8_t instr_code;
    uint8_t r1;
    uint8_t r2;
    uint64_t value;
};

#endif

//
// Created by rdelfin on 3/30/17.
//

#ifndef HW6_LITERAL_H
#define HW6_LITERAL_H

#include <map>
#include <string>

#include <instruction_definitions.h>
#include <tree.h>

bool isLiteral(node* n);

class literal {
public:
    literal();
    literal(node* n);

    void write_to_memory(std::map<y86addr_t, uint8_t>& memMap, y86addr_t* addr);
    size_t getLen();

    ~literal() { }

private:
    std::vector<y86addr_t> data;
    size_t elemLen;

    y86addr_t num_string_to_value(const std::string& value);
};


#endif //HW6_LITERAL_H

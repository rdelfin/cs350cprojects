//
// Created by rdelfin on 3/30/17.
//

#include <instruction.h>
#include <parsing_functions.h>
#include <parser.h>
#include <algorithm>
#include "literal.h"

bool isLiteral(node* n) {
    if (n->isLeaf || n->children.size() <= 1 || !n->children[0]->isLeaf)
        return false;

    std::string name = n->children[0]->value;
    std::transform(name.begin(), name.end(), name.begin(), ::tolower);

    // There was an error and some child is not a leaf
    return name == "qword" || name == "byte";

}

literal::literal() {

}

literal::literal(node* n) {
    if (n->isLeaf || n->children.size() <= 1 || !n->children[0]->isLeaf)
        throw InvalidInstructionException("The instruction is not a valid literal");
    std::vector<node*>::iterator it;
    for (it = n->children.begin() + 1; it != n->children.end() && (*it)->isLeaf; ++it);

    // There was an error and some child is not a leaf
    if (it != n->children.end()) {
        throw InvalidInstructionException("The literal declaration contains parenthesis.");
    }

    std::string name = n->children[0]->value;
    std::transform(name.begin(), name.end(), name.begin(), ::tolower);

    if(name == "qword") {
        this->elemLen = 8;
        for(auto it = n->children.begin() + 1; it != n->children.end(); ++it)
            this->data.push_back(num_string_to_value((*it)->value));
    }

    else if(name == "byte") {
        this->elemLen = 1;
        for(auto it = n->children.begin() + 1; it != n->children.end(); ++it)
            this->data.push_back(num_string_to_value((*it)->value));
    }

    else {
        throw InvalidInstructionException("The literal provided does not exist");
    }
}

size_t literal::getLen() {
    return elemLen * data.size();
}

void literal::write_to_memory(std::map<y86addr_t, uint8_t> &memMap, y86addr_t* addr) {
    for(auto it = data.begin(); it != data.end(); ++it) {

        uint8_t temp_data[elemLen + sizeof(y86addr_t)];
        *((y86addr_t*) temp_data) = *it;

        for(size_t b = 0; b < elemLen; b++)
            memMap[(*addr)++] = temp_data[b];
    }
}

y86addr_t literal::num_string_to_value(const std::string& value) {
    y86addr_t val = 0;

    // For decimal, parse through strtol() with base 10
    if(isDecimalInteger(value))
        val = (y86addr_t)strtol(value.c_str(), nullptr, 10);

        // For hexadecimal, replace '#' with '0x' and pass through strtol()
    else if(isHexInteger(value)) {
        std::stringstream ss;
        for(const char* p = value.c_str(); *p != '\0'; p++) {
            if(*p == '#')
                ss << "0";
            else
                ss.put(*p);
        }
        val = (y86addr_t)strtol(ss.str().c_str(), nullptr, 0);
    }
    else {
        std::stringstream ss;
        ss << "Invalid number format: " << value;
        throw InvalidNumberException(ss.str());
    }

    return val;
}

//
// Created by rdelfin on 3/29/17.
//

#include <parser.h>
#include <instruction.h>
#include <parsing_functions.h>

#include <algorithm>

parser::parser(node *n) : n(n) {

}

void parser::parse() {
    calculateLabels();
    parseBytes();
}

const std::unordered_map<std::string, y86addr_t> parser::getLabels() {
    return labelmap;
}

const std::map<y86addr_t, uint8_t> parser::getMemMap() {
    return bytemap;
}

void parser::calculateLabels() {
    // Initial address is always 0 unless otherwise specified
    y86addr_t address = 0;

    for(auto it = n->children.begin(); it != n->children.end(); ++it) {
        // If directive, parse through this class
        if(isDirective(*it)) {
            parseDirective(*it, &address);
        }
        // This is a label, add to label map
        else if(isLabel(*it)) {
            node* label = *it;
            if(labelmap.count(label->value) != 0) {
                std::stringstream ss;
                ss << "Label " << label->value << " is redefined. This is undefined behaviour";
                throw InvalidLabelException(ss.str());
            }
            labelmap[label->value] = address;
        }
        // Otherwise, use instruction parsing
        else {
            instruction i(*it, std::unordered_map<std::string, y86addr_t>(), true);
            address += i.len;
        }
    }
}

void parser::parseBytes() {
    // Initial address is always 0 unless otherwise specified
    y86addr_t address = 0;


    for(auto it = n->children.begin(); it != n->children.end(); ++it) {
        node* child = *it;
        // If directive, parse through this class
        if(isDirective(*it)) {
            parseDirective(*it, &address);
        }
        // Ignore all labels on second pass
        else if(isLabel(*it)) {
        }
        // Otherwise, use instruction parsing
        else {
            instruction i(*it, labelmap, false);
            i.write_to_memory(bytemap, &address);
        }
    }
}

bool parser::isLabel(node* n) {
    return n->isLeaf;
}
bool parser::isDirective(node *n) {
    if(!n->isLeaf && n->children.size() > 0 && n->children[0]->isLeaf) {
        std::string first = n->children[0]->value;
        std::transform(first.begin(), first.end(), first.begin(), ::tolower);
        return first == "pos" || first == "align" || first == "space";
    }

    return false;
}

void parser::parseDirective(node *n, y86addr_t* startAddr) {
    if(!isDirective(n))
        throw InvalidInstructionException("This is an unrecognized directive.");

    std::string first = n->children[0]->value;
    std::transform(first.begin(), first.end(), first.begin(), ::tolower);

    if(first == "pos") {
        parsePos(n, startAddr);
    } else if(first == "align") {
        parseAlign(n, startAddr);
    } else if(first == "space") {
        parseSpace(n, startAddr);
    }
}

void parser::parseAlign(node *n, y86addr_t* startAddr) {
    if(n->children.size() != 2)
        throw InvalidInstructionException("The align directive should have 2 arguments.");
    if(!n->children[1]->isLeaf)
        throw InvalidInstructionException("The align directive's second argument cannot have any parenthesis");

    y86addr_t val = num_string_to_value(n->children[1]->value);

    // If value is not already aligned, go to the next address divisible by val
    if(*startAddr % val != 0)
        *startAddr = (*startAddr/val + 1) * val;
}

void parser::parsePos(node *n, y86addr_t* startAddr) {
    if(n->children.size() != 2)
        throw InvalidInstructionException("The pos directive should have 2 arguments.");
    if(!n->children[1]->isLeaf)
        throw InvalidInstructionException("The pos directive's second argument cannot have any parenthesis");

    y86addr_t val = num_string_to_value(n->children[1]->value);
    *startAddr = val;
}

void parser::parseSpace(node *n, y86addr_t* startAddr) {
    if(n->children.size() != 2)
        throw InvalidInstructionException("The space directive should have 2 arguments.");
    if(!n->children[1]->isLeaf)
        throw InvalidInstructionException("The space directive's second argument cannot have any parenthesis");

    y86addr_t val = num_string_to_value(n->children[1]->value);
    *startAddr += val;
}

y86addr_t parser::num_string_to_value(const std::string& value) {
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
    else {
        std::stringstream ss;
        ss << "Invalid number format: " << value;
        throw InvalidNumberException(ss.str());
    }
}

parser::~parser() {

}

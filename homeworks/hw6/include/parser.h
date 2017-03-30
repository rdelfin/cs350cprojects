//
// Created by rdelfin on 3/29/17.
//

#ifndef HW6_PARSER_HPP
#define HW6_PARSER_HPP

#include <map>
#include <unordered_map>

#include <tree.h>
#include <instruction_definitions.h>


class parser {
public:
    parser(node *n);

    void parse();

    const std::unordered_map<std::string, y86addr_t> getLabels();
    const std::map<y86addr_t, uint8_t> getMemMap();

    ~parser();
private:
    node* n;
    std::unordered_map<std::string, y86addr_t> labelmap;    // Unordered (hash) map for fast access
    std::map<y86addr_t, uint8_t> bytemap;                   // Ordered (tree) map for fast ordered traversal

    void calculateLabels();
    void parseBytes();

    bool isLabel(node* n);
    bool isDirective(node *n);
    void parseDirective(node *n, y86addr_t*);

    void parseAlign(node *n, y86addr_t*);
    void parsePos(node *n, y86addr_t*);
    void parseSpace(node *n, y86addr_t*);

    y86addr_t num_string_to_value(const std::string&);
};

class InvalidNumberException {
public:
    InvalidNumberException(const std::string& msg) : message(msg) { }
    std::string message;
};


#endif //HW6_PARSER_HPP

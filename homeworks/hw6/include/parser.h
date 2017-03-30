//
// Created by rdelfin on 3/29/17.
//

#ifndef HW6_PARSER_HPP
#define HW6_PARSER_HPP


class parser {
public:
    parser(node *n);

    void parse();




    ~parser();
private:
    node* n;
    std::unordered_map<std::string, y86addr_t> labelmap;    // Unordered (hash) map for fast access
    std::map<y86addr_t, uint8_t> bytemap;                   // Ordered (tree) map for fast ordered traversal
};


#endif //HW6_PARSER_HPP

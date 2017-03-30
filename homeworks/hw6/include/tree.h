//
// Created by rdelfin on 3/27/17.
//

#ifndef HW6_TREE_H
#define HW6_TREE_H

#include <vector>
#include <string>
#include <sstream>
#include <iostream>

class node {
public:
    node();
    node(std::string value);
    node(std::vector<node*> children);
    node(const node& n);

    node& operator=(const node& n);

    ~node();

    bool isLeaf;
    std::vector<node*> children;
    std::string value;

    void print();
    std::string printStr();

private:
    std::string print(int level);
};

class tree {
public:
    tree();
    tree(const std::string& s);
    ~tree() { }

    void print();
    node root;
};

struct ParsingException {
public:
    ParsingException(const std::string& msg) : message(msg) { }

    std::string message;
};


#endif //HW6_TREE_H

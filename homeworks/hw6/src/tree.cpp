//
// Created by rdelfin on 3/27/17.
//

#include "tree.h"

#include <sstream>
#include <stack>
#include <iostream>

bool isWhitespace(int c);
bool skipWhitespace(std::stringstream&);
std::string getToken(std::stringstream&);

node::node() { }

node::node(std::string value)
: isLeaf(true), value(value) { }

node::node(std::vector<node*> children)
: isLeaf(false), children(children) { }

node::node(const node& n) {
    this->isLeaf = n.isLeaf;
    if(this->isLeaf)
        this->value = n.value;
    else
        this->children = n.children;
}

node& node::operator=(const node& n) {
    this->isLeaf = n.isLeaf;
    if(this->isLeaf)
        this->value = n.value;
    else
        this->children = n.children;
}

void node::print() {
    print(0);
}


void node::print(int level) {
    std::stringstream indStream;
    std::string indentation;
    for(int i = 0; i < level; i++)
        indStream.put('\t');
    indentation = indStream.str();

    if(isLeaf) {
        std::cout << indentation << value << std::endl;
    } else {
        std::cout << indentation << "(" << std::endl;
        for(auto it = children.begin(); it != children.end(); ++it) {
            (*it)->print(level + 1);
        }
        std::cout << indentation << ")" << std::endl;
    }
}

node::~node() {
    if(!isLeaf) {
        for(auto it = children.begin(); it != children.end(); ++it) {
            delete *it;
        }
    }
}




tree::tree() {

}


tree::tree(const std::string& s) {
    // Begin parsing tree with stringstream
    std::stringstream stream(s);
    std::stack<node*> nodeStack;

    root = node(std::vector<node*>());
    nodeStack.push(&root);

    while(!stream.eof()) {
        int n = stream.peek();

        // Skip over to next valid token
        if(isWhitespace(n)) {
            skipWhitespace(stream);
        }
        // Begin a new child
        else if(n == '(') {
            stream.get();
            node* newNode = new node(std::vector<node*>());
            nodeStack.top()->children.push_back(newNode);
            nodeStack.push(newNode);
        }
        // End the child
        else if(n == ')') {
            stream.get();
            if(nodeStack.size() <= 1)
                throw ParsingException("Unmatched parenthesis.");
            nodeStack.pop();
        }
        // This is an inner child. Parse accordingly
        else {
            nodeStack.top()->children.push_back(new node(getToken(stream)));
        }
    }

    if(nodeStack.size() != 1) {
        throw ParsingException("Unmatched parenthesis.");
    }
}

void tree::print() {
    root.print();
}

std::string getToken(std::stringstream& inStream) {
    std::stringstream outStream;

    int next = inStream.peek();
    while(next > 31 && !isWhitespace(next) && next != '(' && next != ')' && next != EOF) {
        outStream.put((char)next);
        inStream.get();
        next = inStream.peek();
    }

    return outStream.str();
}

bool skipWhitespace(std::stringstream& s) {
    int n = s.peek();

    while(isWhitespace((char)n)) {
        s.get();
        n = s.peek();
    }
}

bool isWhitespace(int c) {
   return c == ' ' || c == '\n' || c == '\t';
}

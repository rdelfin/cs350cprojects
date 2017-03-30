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
std::string stripComments(const std::string&);

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
        indStream.put('  ');
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
    std::string noComentStr = stripComments(s);

    // Begin parsing tree with stringstream
    std::stringstream stream(noComentStr);
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

std::string stripComments(const std::string& s) {
    std::stringstream text;

    bool in_comment = false;
    for(const char* p = s.c_str(); *p != '\0'; p++) {
        if(!in_comment) {
            if(*p == ';')
                in_comment = true;
            else
                text.put(*p);
        } else if(*p == '\n') {
            in_comment = false;
        }
    }

    return text.str();
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
   return c == ' ' || c == '\n' || c == '\t' || c == '\r';
}

#include <iostream>
#include <vector>
#include <fstream>
#include "Graph.h"


int main(int argc, char* argv[]) {
    /**
     * Reads the graph in for
     */
    if(argc != 2) {
        if(argc <= 1)
            std::cout << "No arguments provided. Provide a file name" << std::endl;
        else
            std::cout << "Too many arguments provided. Provide a file name" << std::endl;
    }

    Graph g(argv[1]);

    g.print();


    return 0;
}


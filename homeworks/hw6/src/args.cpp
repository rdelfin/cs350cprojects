#include <args.h>
#include <streambuf>
#include <fstream>
#include <iostream>

std::string get_content(int argc, char* argv[]) {
    if(argc != 2) {
        std::cerr << "Usage: " << argv[0] << " [input-file]" << std::endl;
        std::cerr << " The input file provided is the y86 assembly program to be compiled" << std::endl;
    }

    std::ifstream file(argv[1]);

    return std::string(std::istreambuf_iterator<char>(file),
                       std::istreambuf_iterator<char>());
}


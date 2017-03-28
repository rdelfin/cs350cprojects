#include <cstdlib>
#include <cstdio>
#include <string>
#include <args.h>
#include <tree.h>

int main(int argc, char* argv[]) {
    std::string fileContent = get_content(argc, argv);

    try {
        tree t(fileContent);

        t.print();
    } catch(ParsingException p) {
        std::cerr << "There was an error parsing the file." << std::endl;
        std::cerr << "\t" << p.message << std::endl;
        return -1;
    }

}

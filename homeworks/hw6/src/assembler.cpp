#include <cstdlib>
#include <cstdio>
#include <string>
#include <args.h>
#include <tree.h>

int main(int argc, char* argv[]) {
    std::string fileContent = get_content(argc, argv);

    tree t(fileContent);

    t.print();
}

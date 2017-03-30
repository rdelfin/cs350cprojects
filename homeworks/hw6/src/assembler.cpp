#include <cstdlib>
#include <cstdio>
#include <unordered_map>
#include <string>

#include <args.h>
#include <instruction.h>
#include <tree.h>
#include <parser.h>

int main(int argc, char* argv[]) {
    std::string fileContent = get_content(argc, argv);
    std::string sample_instruction = "(rrmovq %rbx %rcx)";

    try {
        tree t(fileContent);

        parser p(t.root.children[0]);
        p.parse();

        std::map<y86addr_t, uint8_t> memory = p.getMemMap();
        std::cout << "(" << std::endl;
        for(auto it = memory.begin(); it != memory.end(); ++it) {
            std::cout << "\t(" << std::hex << it->first << ", " << it->second << std::dec << ")" << std::endl;
        }

        std::cout << ")" << std::endl;

    } catch(ParsingException p) {
        std::cerr << "There was an error parsing the file." << std::endl;
        std::cerr << "\t" << p.message << std::endl;
        return -1;
    } catch (InvalidInstructionException e) {
        std::cerr << "Invalid instruction exception: \n" << e.message << std::endl;
    } catch (InvalidLabelException e) {
        std::cerr << "Invalid label exception: \n" << e.message << std::endl;
    }

}

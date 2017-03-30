#include <cstdlib>
#include <cstdio>
#include <unordered_map>
#include <string>

#include <args.h>
#include <instruction.h>
#include <tree.h>

int main(int argc, char* argv[]) {
    std::string fileContent = get_content(argc, argv);
    std::string sample_instruction = "(rrmovq %rbx %rcx)";

    try {
        tree t(sample_instruction);

        t.print();

        instruction i(t.root.children[0], std::unordered_map<std::string, y86addr_t>(), true);
        std::cout << "INSTRUCTION: \n"
                  << "\tCode: " << (int)i.instr_code << "\n"
                  << "\tR1: " << (int)i.r1 << "\n"
                  << "\tR2: " << (int)i.r2 << std::endl;

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

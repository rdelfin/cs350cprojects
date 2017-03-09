//
// Created by rdelfin on 3/8/17.
//

#ifndef HW6_ASMCODE_H
#define HW6_ASMCODE_H

#include <cstdint>
#include <string>

class asmcode {
public:
    asmcode(std::string file);

    ~asmcode();
private:
    std::unordered_map<y86addr_t, instruction> instruction_map;
};


#endif //HW6_ASMCODE_H

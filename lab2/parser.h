//
// Created by rdelfin on 4/3/17.
//

#ifndef LAB2_PARSE_H
#define LAB2_PARSE_H

#include <stdio.h>

#include "state.h"

state_t* parse(char* file_name);
int parse_memory_addr(FILE* f, y86addr_t* addr, uint8_t* value);
int parse_token(FILE* f, char* buffer, int buffer_size);
int parse_skip_whitespace(FILE *f);


#endif //LAB2_PARSE_H

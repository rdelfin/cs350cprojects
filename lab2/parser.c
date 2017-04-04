//
// Created by rdelfin on 4/3/17.
//

#include "parser.h"


#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#include "state.h"
#include "parsing_functions.h"

state_t* parse(char * file_name) {
    state_t* state = state_init();

    if(state == NULL) {
        fprintf(stderr, "There was an error creating the state.\n");
        return NULL;
    }

    int parenthesis_lvl = 0;
    bool comment = false;
    char c;

    FILE* f = fopen(file_name, "r");

    while((c = (char)fgetc(f)) != EOF) {
        if(!comment) {
            if (c == '(') {
                parenthesis_lvl++;
                if(parenthesis_lvl > 2) {
                    fprintf(stderr, "There was an error parsing the file. There can only be two levels of parenthesis\n");
                    state_destroy(state);
                    return NULL;
                }

                if(parenthesis_lvl == 2) {
                    y86addr_t address;
                    uint8_t value;
                    fseek(f, -1, SEEK_CUR);
                    if(parse_memory_addr(f, &address, &value)) {
                        state_destroy(state);
                        return NULL;
                    }

                    if(state_write_memory(state, address, value)) {
                        fprintf(stderr, "There was an unknown error writing to the state memory.\n");
                        state_destroy(state);
                        return NULL;
                    }

                    state_write_memory(state, address, value);
                }
            }

            else if(c == ')') {
                parenthesis_lvl--;
                if(parenthesis_lvl < 0) {
                    fprintf(stderr, "There was an error parsing the file. You have too many closing parenthesis.\n");
                    state_destroy(state);
                    return NULL;
                }
            }

            else if(c == ';')
                comment = true;

            else if(c == ' ' || c == '\n' || c == '\t' || c == '\r') {
                // Ignore
            }

            else {
                fprintf(stderr, "There was an invalid token when parsing the file.\n");
                state_destroy(state);
                return NULL;
            }
        }


        else if(c == '\n')
            comment = false;
    }

    if(parenthesis_lvl != 0) {
        fprintf(stderr, "There were unmatched parenthesis. The file for any missing closng parenthesis.\n");
        state_destroy(state);
        return NULL;
    }

    return state;
}

int parse_memory_addr(FILE* f, y86addr_t* addr, uint8_t* value) {
    if((char)fgetc(f) != '(') {
        fprintf(stderr, "Expected an opening parenthesis.\n");
        return -1;
    }
    char addr_buff[32], middle_buff[32], data_buff[32];

    parse_skip_whitespace(f);
    parse_token(f, addr_buff, 32);
    parse_skip_whitespace(f);
    parse_token(f, middle_buff, 32);
    parse_skip_whitespace(f);
    parse_token(f, data_buff, 32);
    parse_skip_whitespace(f);
    char endParenthesis = (char)fgetc(f);

    if(endParenthesis != ')') {
        fprintf(stderr, "Expected a closing parenthesis.\n");
        return -1;
    }

    if(strcmp(middle_buff, ".") != 0) {
        fprintf(stderr, "Expected middle token of ( %s %s %s ) to be '.'\n", addr_buff, middle_buff, data_buff);
        return -2;
    }

    uint64_t tempVal;
    y86addr_t tempAddr;
    if(my_strtol(addr_buff, &tempAddr)) {
        fprintf(stderr, "there was an error parsing an address. \"%s\" is not a valid number.\n", addr_buff);
        return -3;
    }

    if(my_strtol(data_buff, &tempVal)) {
        fprintf(stderr, "there was an error parsing a memory value. \"%s\" is not a valid number.\n", data_buff);
        return -3;
    }

    *value = (uint8_t)tempVal;
    *addr = tempAddr;

    fseek(f, -1, SEEK_CUR);

    return 0;
}

int parse_token(FILE* f, char* buffer, int buffer_size) {
    char c = (char)fgetc(f);
    int i = 0;
    while(c != EOF && c != ' ' && c != ')' && c != ';') {
        i++;
        if(i < buffer_size)
            *(buffer++) = c;
        c = (char)fgetc(f);
    }

    *buffer = '\0';

    fseek(f, -1, SEEK_CUR);
    return 0;
}

int parse_skip_whitespace(FILE *f) {
    char c = (char)fgetc(f);
    while(c != EOF && (c == ' ' || c == '\t' || c == '\n' || c == '\r'))
        c = (char)fgetc(f);

    fseek(f, -1, SEEK_CUR);
}

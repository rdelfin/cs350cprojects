//
// Created by rdelfin on 3/30/17.
//

#ifndef HW6_PARSING_FUNCTIONS_H
#define HW6_PARSING_FUNCTIONS_H

#include <ctype.h>
#include <stdlib.h>

static inline bool is_whitespace(char c) {
    return c == ' ' || c == '\t' || c == '\n' || c == '\r';
}

static inline bool isDecimalInteger(char* s)
{
    if(strlen(s) == 0 || ((!isdigit(s[0])) && (s[0] != '-') && (s[0] != '+')))
        return false;

    for(const char* p = s+1; *p != '\0'; p++)
        if(!isdigit(*p))
            return false;

    return true;
}

static inline bool ishexdigit(const char c) {
    return (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F') || (c >= '0' && c <= '9');
}

static inline bool isHexInteger(char* s) {
    if(strlen(s) == 0 || !((s[0] == '#' && s[1] == 'x') || ((s[0] == '-' || s[0] == '+') && (s[1] == '#' && s[2] == 'x') )))
        return false;

    for(const char* p = s + (s[0] == '#' ? 2 : 3); *p != '\0'; p++)
        if(!ishexdigit(*p))
            return false;

    return true;
}

static inline int my_strtol(char* s, uint64_t* data) {
    char* copy = malloc(strlen(s));
    strncpy(copy, s, strlen(s));
    if(isHexInteger(s)) {
        if(s[0] == '#') s[0] = '0';
        if(s[1] == '#') s[1] = '0';
    } else if(!isDecimalInteger(s)) {
        return -1;
    }

    *data = strtol(s, NULL, 0);
    return 0;
}


#endif //HW6_PARSING_FUNCTIONS_H

//
// Created by rdelfin on 3/30/17.
//

#ifndef HW6_PARSING_FUNCTIONS_H
#define HW6_PARSING_FUNCTIONS_H

inline bool is_whitespace(char c) {
    return c == ' ' || c == '\t' || c == '\n' || c == '\r';
}

inline bool isDecimalInteger(const std::string &s)
{
    if(s.empty() || ((!isdigit(s[0])) && (s[0] != '-') && (s[0] != '+')))
        return false;

    for(const char* p = s.c_str()+1; *p != '\0'; p++)
        if(!isdigit(*p))
            return false;

    return true;
}

inline bool ishexdigit(const char c) {
    return (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F') || (c >= '0' && c <= '9');
}

inline bool isHexInteger(const std::string &s) {
    if(s.empty() || !((s[0] == '#' && s[1] == 'x') || ((s[0] == '-' || s[0] == '+') && (s[1] == '#' && s[2] == 'x') )))
        return false;

    for(const char* p = s.c_str() + (s[0] == '#' ? 2 : 3); *p != '\0'; p++)
        if(!ishexdigit(*p))
            return false;

    return true;
}

#endif //HW6_PARSING_FUNCTIONS_H

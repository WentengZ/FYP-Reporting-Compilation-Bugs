#include <fstream>
#include <string>
#include <stdlib.h>
#include <iostream>
#include <vector>

#ifndef INCLUDE_PARSER
#define INCLUDE_PARSER
#include "../include/parser.h"
#endif

int Parser::find_delimiter(std::string text, char delimiter) {
    for (int i = 0; i < text.length(); i++){
        if (text.at(i) == delimiter) {
            return i;
        }
    }
    return -1;
}


std::vector<std::string> split(std::string s, char c) {
    std::vector<std::string> result;
    int previous_pos = 0;
    int i = 0;
    for (i = 0; i < s.length(); i++) {
        if (s.at(i) == c) {
            result.push_back(s.substr(previous_pos, i));
            previous_pos = i+1;
        }
    }
    if (i != previous_pos) {
        result.push_back(s.substr(previous_pos, i));
    }
    return result;
}

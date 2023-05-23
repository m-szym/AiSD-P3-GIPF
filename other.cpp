//
// Created by M Sz on 23/05/2023.
//
#include "other.h"

std::vector<std::string> split_string(const std::string& str) {
    std::istringstream iss(str);
    std::vector<std::string> tokens(std::istream_iterator<std::string>{iss},
                                    std::istream_iterator<std::string>());
    return tokens;
}

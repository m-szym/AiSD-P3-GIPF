//
// Created by M Sz on 17/05/2023.
//

#ifndef GIPF_CONSTANTS_H
#define GIPF_CONSTANTS_H

const std::string SPACER = " ";
const int GIPF_INDEX_START_INT = 1;
const char GIPF_INDEX_START_CHAR = 'A';


const char EMPTY_PLACE_SYMBOL = '_';
const char WHITE_SYMBOL = 'W';
const char BLACK_SYMBOL = 'B';
const char DOT_SYMBOL = '*';
const char WHITE_TARGET_OF_BLACK_SYMBOL = '{';
const char BLACK_TARGET_OF_WHITE_SYMBOL = '}';
const char WHITE_TARGET_OF_WHITE_SYMBOL = '[';
const char BLACK_TARGET_OF_BLACK_SYMBOL = ']';
const char EMPTY_TARGET_SYMBOL = 'e';

const std::string BAD_MOVE_ERROR = "illegal move";
const std::string BAD_COORDINATES_ERROR = "illegal coordinates";
const std::string BAD_COMMAND_FORMAT_ERROR = "illegal command format";
const std::string BAD_COMMAND_ERROR = "illegal command";

#endif //GIPF_CONSTANTS_H

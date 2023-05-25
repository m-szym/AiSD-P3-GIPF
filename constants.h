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
const char USED_DOT_SYMBOL = '#';
const char WHITE_TARGET_OF_BLACK_SYMBOL = '{';
const char BLACK_TARGET_OF_WHITE_SYMBOL = '}';
const char WHITE_TARGET_OF_WHITE_SYMBOL = '[';
const char BLACK_TARGET_OF_BLACK_SYMBOL = ']';
const char EMPTY_TARGET_SYMBOL = 'e';
const char ERROR_SYMBOL = 'X';

const std::string BAD_MOVE_ERROR = "illegal move";
const std::string BAD_COORDINATES_ERROR = "illegal coordinates";
const std::string BAD_COMMAND_FORMAT_ERROR = "illegal command format";
const std::string BAD_COMMAND_ERROR = "illegal command";

const std::string LOAD_BOARD_COMMAND = "LOAD_GAME_BOARD";

const std::string INVALID_BOARD_WHITE_PIECES_ERROR = "WRONG_WHITE_PAWNS_NUMBER";
const std::string INVALID_BOARD_BLACK_PIECES_ERROR = "WRONG_BLACK_PAWNS_NUMBER";
const std::string INVALID_BOARD_ROW_ERROR = "WRONG_BOARD_ROW_LENGTH";
const std::string BAD_MOVE_START = "BAD_MOVE_";
const std::string BAD_MOVE_IS_WRONG_INDEX = "_IS_WRONG_INDEX";

const std::string BOARD_LOAD_OK = "BOARD_STATE_OK";

#endif //GIPF_CONSTANTS_H

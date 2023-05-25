//
// Created by M Sz on 17/05/2023.
//

#include "GIPF.h"

GIPF::GIPF(int size, int killing_number, int white_pieces, int black_pieces)
    : hex_coords{}, coords_hex{}, board(size), size(size), killing_number(killing_number), base_white_pieces(white_pieces),
      base_black_pieces(black_pieces), prisoners_of_white(0), prisoners_of_black(0), white_reserve(0), black_reserve(0),
      next_player(BLACK_SYMBOL), current_player(WHITE_SYMBOL)
{
    board.construct_map(hex_coords, coords_hex);
    board.read_map();
}

GIPF::GIPF(int size, int killing_number, int white_pieces, int black_pieces, int white_reserve, int black_reserve, char starting_player)
        : hex_coords{}, coords_hex{}, board(size), size(size), killing_number(killing_number), base_white_pieces(white_pieces),
          base_black_pieces(black_pieces), prisoners_of_white(0), prisoners_of_black(0), white_reserve(white_reserve), black_reserve(black_reserve)
{
    if (starting_player == WHITE_SYMBOL) {
        next_player = WHITE_SYMBOL;
        current_player = BLACK_SYMBOL;
    } else {
        next_player = BLACK_SYMBOL;
        current_player = WHITE_SYMBOL;
    }

    board.construct_map(hex_coords, coords_hex);
    board.read_map();

    check_map();
}

GIPF::GIPF(int params[], char starting_player, std::vector<char> flat_board)
        : board(params[0]) {
    size = params[0];
    killing_number = params[1];
    base_white_pieces = params[2];
    base_black_pieces = params[3];
    white_reserve = params[4];
    black_reserve = params[5];
    current_player = starting_player;
    next_player = (starting_player == WHITE_SYMBOL) ? BLACK_SYMBOL : WHITE_SYMBOL;

    board.construct_map(hex_coords, coords_hex);
    board.new_read_map(flat_board);

    //board.print_cont();
    //board.print_gipf();
}

void GIPF::check_map() const {

}

//teraz tylko wczytuje - nie sprawdza poprawności (poza translate)
std::vector<Hex> GIPF::read_move() {
    std::string dot_position;
    std::string entry_position;

    std::string i;
    std::cin >> i;
    dot_position = i.substr(0, 2);
    char dash = i[2];
    entry_position = i.substr(3, 2);
//    std::cin >> dot_position;
//    int dash = getchar();
//    std::cin >> entry_position;

    if (dot_position.length() == 2 && entry_position.length() == 2 && dash == '-') {
        try {
            Hex dot = translate(dot_position);                              //ewntualnie sprawdzic czy nie EMPTY_HEX
            if (dot == EMPTY_HEX) {
                std::cout << "BAD_MOVE_" << dot_position << "_IS_WRONG_INDEX" << std::endl;
                return std::vector({EMPTY_HEX, EMPTY_HEX});
            }
            try {
                Hex entry = translate(entry_position);
                if (entry == EMPTY_HEX) {
                    std::cout << "BAD_MOVE_" << entry_position << "_IS_WRONG_INDEX" << std::endl;
                    return std::vector({EMPTY_HEX, EMPTY_HEX});
                }

                std::vector move = {dot, entry};

                std::string details;
                std::getline(std::cin, details);
                if (details.length() > 0) {
                    if (read_details(details, move)) {
//                        do debugowania
//                        for (auto hex : move) {
//                            std::cout << (char) board[hex] << std::endl;
//                        }
                        return move;
                    } else {
                        std::cout << BAD_COMMAND_ERROR << "=" << details << "|" << std::endl;
                        return std::vector({EMPTY_HEX, EMPTY_HEX});
                    }
                } else {
                    return move;
                }
            } catch (std::out_of_range& e) {
                std::cout << "BAD_MOVE_" << entry_position << "_IS_WRONG_INDEX" << std::endl;
                return std::vector({EMPTY_HEX, EMPTY_HEX});
            }
        } catch (std::out_of_range& e) {
            std::cout << "BAD_MOVE_" << dot_position << "_IS_WRONG_INDEX" << std::endl;
            return std::vector({EMPTY_HEX, EMPTY_HEX});
        }
    }

    std::cout << BAD_COMMAND_FORMAT_ERROR << "dot=" << dot_position << "| dash=" << (char) dash << "| entry=" << entry_position << "|" << std::endl;
    return std::vector({EMPTY_HEX, EMPTY_HEX});
}



void GIPF::make_move(std::vector<Hex> move) {
    if (!is_valid_move_basic(move)) {
        //std::cout << BAD_MOVE_ERROR << std::endl;
        return;
    }

    int dir = move[0].neighbour_direction(move[1]);
    if (dir < 0 || dir >= HEX_DIRECTIONS_COUNT) {
        std::cout << "UNKNOWN_MOVE_DIRECTION" << std::endl;
        return;
    }
    auto path = board.get_fullline(move[0], dir);
    if (!board.line_has_empty_hex(path)) {
        std::cout << "BAD_MOVE_ROW_IS_FULL" << std::endl;
        return;
    }
    board.push_line(path, current_player);
    //std::cout << "\t" << current_player << "inserted at " << translate(path.front()) << std::endl;
    if (current_player == WHITE_SYMBOL) white_reserve--;
    else black_reserve--;
    std::cout << "MOVE_COMMITTED" << std::endl;

    //board.set(move[0], DOT_SYMBOL);
    next_player = (next_player == WHITE_SYMBOL) ? BLACK_SYMBOL : WHITE_SYMBOL;
    current_player = (current_player == WHITE_SYMBOL) ? BLACK_SYMBOL : WHITE_SYMBOL;

}

bool GIPF::is_valid_move_basic(std::vector<Hex> move) {
    if (move[0] == EMPTY_HEX) {
        //std::cout << "BAD_MOVE_" << translate(move[0]) << "_IS_WRONG_INDEX" << std::endl;
        return false;
    }

    if (move[1] == EMPTY_HEX) {
        //std::cout << "BAD_MOVE_" << translate(move[1]) << "_IS_WRONG_INDEX" << std::endl;
        return false;
    }

    if (move[0].distance(move[1]) != 1) {
        std::cout << "UNKNOWN_MOVE_DIRECTION" << std::endl;
        return false;
    }

    if (board[move[0]] != DOT_SYMBOL) {
        std::cout << "BAD_MOVE_" << translate(move[0]) << "_IS_WRONG_STARTING_FIELD" << std::endl;
        return false;
    }

    if (board[move[1]] != WHITE_SYMBOL && board[move[1]] != BLACK_SYMBOL && board[move[1]] != EMPTY_PLACE_SYMBOL) {
        std::cout << "BAD_MOVE_" << translate(move[1]) << "_IS_WRONG_DESTINATION_FIELD" << std::endl;
        return false;
    }

    return true;
}

void GIPF::simple_move() {
    auto move = read_move();
    make_move(move);
    //evaluate_turn(move);
}

bool GIPF::mark_target_line(const std::vector<Hex>& line) {
    if (line.size() < killing_number) {
        std::cout << "too small line" << std::endl;     //TODO: remove
        return false;
    }

    int white_line = 0;
    int black_line = 0;
    bool col = false;
    for (auto hex : line) {
        if (white_line >= killing_number || black_line >= killing_number)
            break;

        if (board[hex] == WHITE_SYMBOL || board[hex] == WHITE_TARGET_OF_BLACK_SYMBOL
                || board[hex] == WHITE_TARGET_OF_WHITE_SYMBOL) {
            white_line++;
            if (black_line) black_line = 0;
        } else if (board[hex] == BLACK_SYMBOL || board[hex] == BLACK_TARGET_OF_BLACK_SYMBOL
                || board[hex] == BLACK_TARGET_OF_WHITE_SYMBOL) {
            black_line++;
            if (white_line) white_line = 0;
        } else if (board[hex] == EMPTY_PLACE_SYMBOL){
            if (white_line) white_line = 0;
            if (black_line) black_line = 0;
        }

        if (board[hex] == WHITE_TARGET_OF_BLACK_SYMBOL || board[hex] == WHITE_TARGET_OF_WHITE_SYMBOL
                || board[hex] == BLACK_TARGET_OF_BLACK_SYMBOL || board[hex] == BLACK_TARGET_OF_WHITE_SYMBOL) {
            col = true;
//        } else {
//            std::cout << "marking error at " << translate(hex) << " char = " << board[hex] << std::endl;
//            return false;
        }
    }

    if (white_line < killing_number && black_line < killing_number) {
        return false;
    }

    if (col) {
        std::cout << "TUTAJ" << std::endl;
        for (auto x : line) {
            std::cout << "\t" << translate(x) << " ";
        }
        std::cout << "\n";
        return true;
    }

    std::cout << "killing line" << std::endl;

    for (auto hex : line) {
        if (board[hex] == WHITE_SYMBOL || board[hex] == BLACK_SYMBOL || board[hex] == EMPTY_PLACE_SYMBOL) {     //ie is not already part of target line
            std::cout << "marking " << translate(hex) << " --> ";
            if (white_line >= killing_number) {
                if (board[hex] == BLACK_SYMBOL) {
                    std::cout << "b->w at " << translate(hex) << std::endl;
                    board.set(hex, BLACK_TARGET_OF_WHITE_SYMBOL);
                } else if (board[hex] == WHITE_SYMBOL) {
                    std::cout << "w->w at " << translate(hex) << std::endl;
                    board.set(hex, WHITE_TARGET_OF_WHITE_SYMBOL);
                } else if (board[hex] == EMPTY_PLACE_SYMBOL){
                    //board.set(hex, EMPTY_TARGET_SYMBOL);
                    std::cout << "_->_ at " << translate(hex) << std::endl;
                } else {
                    std::cout << "error char at " << translate(hex) << " == " << board[hex] << std::endl;
                }
            } else if (black_line >= killing_number) {
                if (board[hex] == WHITE_SYMBOL) {
                    std::cout << "w->b at " << translate(hex) << std::endl;
                    board.set(hex, WHITE_TARGET_OF_BLACK_SYMBOL);
                } else if (board[hex] == BLACK_SYMBOL) {
                    std::cout << "b->b at " << translate(hex) << std::endl;
                    board.set(hex, BLACK_TARGET_OF_BLACK_SYMBOL);
                }
            }

        } else {
            //przecięcie
            std::cout << "kolizja " << translate(hex) << std::endl;
            return true;
        }
    }
    return false;
}

bool GIPF::read_details(const std::string& details, std::vector<Hex>& move) {
    std::vector<std::string> tokens = split_string(details);

    if(tokens.empty()) {    //chyba ok
        return true;
    }

    if (tokens[0].size() != 1) {
        return false;
    }

    if(tokens[0] == "b" || tokens[0] == "w") {
        //std::cout << "b/w" << std::endl;
        auto it = tokens.begin() + 1;
        while (it != tokens.end()) {
            try {
                //Hex hex = board.key_map.at(*it);
                Hex hex = translate(*it);
                if (board[hex] != DOT_SYMBOL) {
                    move.push_back(hex);
                } else {
                    return false;
                }
            } catch (std::out_of_range& e) {
                std::cout << BAD_COORDINATES_ERROR << std::endl;
                return false;
            }
            it++;
        }
        return true;
    }


    return false;
}


std::string GIPF::translate(const Hex &hex) {
    try {
        return hex_coords.at(hex);
    } catch (std::out_of_range& e) {
        return "XXX";
    }
}

Hex GIPF::translate(const std::string &coords) {
    try {
        return coords_hex.at(coords);
    } catch (std::out_of_range &e) {
        return EMPTY_HEX;
    }
}

void GIPF::evaluate_turn(const std::vector<Hex> &last_move) {
    bool special_move = last_move.size() > 2;
    board.print_cont();
    auto dotlines = board.get_dotlines();
    if (dotlines.empty()) {
        std::cout << "no dotlines" << std::endl;
        return;
    }

    if (special_move) {
        std::cout << "special move" << std::endl;
        auto it = last_move.begin() + 2;
        while (it != last_move.end()) {
            if (current_player == WHITE_SYMBOL) {
                if (board[*it] == BLACK_SYMBOL) {
                    white_reserve++;
                } else if (board[*it] == BLACK_SYMBOL) {
                    prisoners_of_white++;
                }
                board.set(*it, EMPTY_PLACE_SYMBOL);
            } else if (current_player == BLACK_SYMBOL) {
                if (board[*it] == BLACK_SYMBOL) {
                    black_reserve++;
                } else if (board[*it] == WHITE_SYMBOL) {
                    prisoners_of_black++;
                }
                board.set(*it, EMPTY_PLACE_SYMBOL);
            }
            it++;
        }
    }

    int i = 0;
    int v = 0;
    for (const auto& line : dotlines) {
//        if (board.line_was_visited(line)) {
//            v++;
//            //std::cout << "line " << i << " was visited" << std::endl;
//            std::cout << "line was visited == collision" << std::endl;
//            for (auto x : line) {
//                std::cout << "\t" << translate(x) << " ";
//            }
//            std::cout << std::endl;
//            continue;
//        }
        if(mark_target_line(line)) {
            std::cout << "kolizja" << std::endl;
            break;
        }
        i++;
    }
    //std::cout << "v=" << v << "_of_i=" << i << "_" << std::endl;

    board.print_cont();
    kill_targets(dotlines);
    board.print_cont();
    board.clear_board();
}

void GIPF::kill_targets(std::vector<std::vector<Hex>> &dotlines) {
    for (const auto& line : dotlines) {
        for (const auto& hex : line) {
            if (board[hex] == WHITE_TARGET_OF_WHITE_SYMBOL || board[hex] == WHITE_TARGET_OF_BLACK_SYMBOL
             || board[hex] == BLACK_TARGET_OF_WHITE_SYMBOL || board[hex] == BLACK_TARGET_OF_BLACK_SYMBOL) {

                if (board[hex] == WHITE_TARGET_OF_WHITE_SYMBOL) {
                    white_reserve++;
                } else if (board[hex] == BLACK_TARGET_OF_WHITE_SYMBOL) {
                    prisoners_of_white++;
                } else if (board[hex] == WHITE_TARGET_OF_BLACK_SYMBOL) {
                    prisoners_of_black++;
                } else if (board[hex] == BLACK_TARGET_OF_BLACK_SYMBOL) {
                    black_reserve++;
                }

                board.set(hex, EMPTY_PLACE_SYMBOL);
            }
        }
    }
}

void GIPF::print_game_state() const {
    std::cout << size << " " << killing_number << " " << base_white_pieces << " " << base_black_pieces << std::endl;
    std::cout << white_reserve << " " << black_reserve << " " << current_player << std::endl;
    board.print_gipf();
//    std::cout<< std::endl;
//    board.print_cont();
//    std::cout<< std::endl;
//    board.print_xN();
//    std::cout<< std::endl;
}




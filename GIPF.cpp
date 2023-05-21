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


std::vector<Hex> GIPF::read_move() {
    std::string dot_position;
    std::string entry_position;

    //std::getline(std::cin, dot_position, ' ');
    std::cin >> dot_position;
    if (dot_position.length() == 2) {
        std::cout << "d-" << dot_position << "+";
        getchar();
        if (getchar() == '-') {
            getchar();
            std::cout << "_-_";
            //std::getline(std::cin, entry_position);
            std::cin >> entry_position;
            if (entry_position.length() == 2) {
                std::cout << "e-" << entry_position << "+" << std::endl;

                try {
                    //Hex dot = board.key_map.at(dot_position);
                    Hex dot = translate(dot_position);
                    dot.print();
                    std::cout << (char) board[dot] << std::endl;
                    try {
                        //Hex entry = board.key_map.at(entry_position);
                        Hex entry = translate(entry_position);
                        entry.print();
                        std::cout << (char) board[entry] << std::endl;

                        std::vector move = {dot, entry};
                        std::string details;
                        std::getline(std::cin, details);
                        std::cout << "d=" << details << "|" << std::endl;
                        if (details.length() > 0) {
                            if (read_details(details, move)) {
                                for (auto hex : move) {
                                    std::cout << (char) board[hex] << std::endl;
                                }
                                return move;
                            } else {
                                std::cout << BAD_COMMAND_ERROR << std::endl;
                                return std::vector({EMPTY_HEX, EMPTY_HEX});
                            }
                        } else {
                            return move;
                        }
                    } catch (std::out_of_range& e) {
                        std::cout << BAD_COORDINATES_ERROR << std::endl;
                        return std::vector({EMPTY_HEX, EMPTY_HEX});
                    }
                } catch (std::out_of_range& e) {
                    std::cout << BAD_COORDINATES_ERROR << std::endl;
                    return std::vector({EMPTY_HEX, EMPTY_HEX});
                }
            }
        }
    }

    std::cout << BAD_COMMAND_FORMAT_ERROR << std::endl;
    return std::vector({EMPTY_HEX, EMPTY_HEX});
}



void GIPF::make_move(std::vector<Hex> move) {
    if (move[0] == EMPTY_HEX || move[1] == EMPTY_HEX) {
        return;
    }

    //board.set(move[0], current_player);

    if (is_valid_move_basic(move)) {
        int dir = move[0].neighbour_direction(move[1]);
        auto path = board.get_fullline(move[0], dir);
        if (board.line_has_empty_hex(path)) {
            board.push_line(path, current_player);
        }
    } else {
        std::cout << BAD_MOVE_ERROR << std::endl;
        return;
    }

    //board.set(move[0], DOT_SYMBOL);
    next_player = (next_player == WHITE_SYMBOL) ? BLACK_SYMBOL : WHITE_SYMBOL;

}

bool GIPF::is_valid_move_basic(std::vector<Hex> move) {
    if (move[0] == EMPTY_HEX || move[1] == EMPTY_HEX) {
        return false;
    }

    if (move[0].distance(move[1]) != 1) {
        return false;
    }



    return true;
}

void GIPF::simple_move() {
    auto move = read_move();
    make_move(move);
    evaluate_turn(move);
}

bool GIPF::mark_target_line(const std::vector<Hex>& line) {
    if (line.size() < killing_number) {
        return false;
    }

    int white_line = 0;
    int black_line = 0;
    for (auto hex : line) {
        if (white_line >= killing_number || black_line >= killing_number)
            break;

        if (board[hex] == WHITE_SYMBOL) {
            white_line++;
            if (black_line) black_line = 0;
        } else if (board[hex] == BLACK_SYMBOL) {
            black_line++;
            if (white_line) white_line = 0;
        } else {
            if (white_line) white_line = 0;
            if (black_line) black_line = 0;
        }
    }

    if (white_line < killing_number && black_line < killing_number) {
        //std::cout << "no killing line" << std::endl;
        return false;
    }

    for (auto hex : line) {
        if (board[hex] == WHITE_SYMBOL || board[hex] == BLACK_SYMBOL || board[hex] == EMPTY_PLACE_SYMBOL) {     //ie is not already part of target line
            if (white_line >= killing_number) {
                if (board[hex] == BLACK_SYMBOL) {
                    board.set(hex, BLACK_TARGET_OF_WHITE_SYMBOL);
                } else if (board[hex] == WHITE_SYMBOL) {
                    board.set(hex, WHITE_TARGET_OF_WHITE_SYMBOL);
                } else {
                    //board.set(hex, EMPTY_TARGET_SYMBOL);
                }
            } else if (black_line >= killing_number) {
                if (board[hex] == WHITE_SYMBOL) {
                    board.set(hex, WHITE_TARGET_OF_BLACK_SYMBOL);
                } else if (board[hex] == BLACK_SYMBOL) {
                    board.set(hex, BLACK_TARGET_OF_BLACK_SYMBOL);
                } else {
                    //board.set(hex, EMPTY_TARGET_SYMBOL);
                }
            //} else if (board[hex] == EMPTY_PLACE_SYMBOL){
                //todo
            } else {
                //przecięcie
                std::cout << "tu kolizja" << std::endl;
                return true;
            }
        }
    }
    return false;
}

bool GIPF::read_details(const std::string& details, std::vector<Hex>& move) {

    std::istringstream iss(details);
    std::vector<std::string> tokens(std::istream_iterator<std::string>{iss},
                                     std::istream_iterator<std::string>());

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
        return "";
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

    auto dotlines = board.get_dotlines();
    if (dotlines.size() == 0) {
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
    for (const auto& line : dotlines) {
        if (board.line_was_visited(line)) {
            std::cout << "line " << i << " was visited" << std::endl;
            continue;
        }
        if(mark_target_line(line)) {
            std::cout << "kolizja" << std::endl;
            break;
        }
        i++;
    }

    kill_targets(dotlines);

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


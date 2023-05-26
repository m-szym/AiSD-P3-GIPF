//
// Created by M Sz on 17/05/2023.
//

#include "GIPF.h"

GIPF::GIPF(int size, int killing_number, int white_pieces, int black_pieces, int white_reserve, int black_reserve, char starting_player)
        : hex_coords{}, coords_hex{}, board(size), size(size), killing_number(killing_number), base_white_pieces(white_pieces),
          base_black_pieces(black_pieces), prisoners_of_white(0), prisoners_of_black(0), white_reserve(white_reserve), black_reserve(black_reserve)
{
    current_player = starting_player;
    next_player = (starting_player == WHITE_SYMBOL) ? BLACK_SYMBOL : WHITE_SYMBOL;

    board.construct_map(hex_coords, coords_hex);
    board.read_map();

    check_map();
}

GIPF::GIPF(int params[], char starting_player, std::vector<char> flat_board, bool &good_state)
        : board(params[0]) {
    size = params[0];
    killing_number = params[1];

    base_white_pieces = params[2];
    base_black_pieces = params[3];
    white_reserve = params[4];
    black_reserve = params[5];
    prisoners_of_white = 0;
    prisoners_of_black = 0;

    current_player = starting_player;
    next_player = (starting_player == WHITE_SYMBOL) ? BLACK_SYMBOL : WHITE_SYMBOL;

    board.construct_map(hex_coords, coords_hex);
    board.new_read_map(std::move(flat_board));
    good_state = check_map();
    board.clear_board();
}

bool GIPF::check_map() {
    auto dotlines = board.get_dotlines();
    int incorrect_rows = 0;
    int white_row = 0;
    int black_row = 0;

    for (const auto& line : dotlines) {
        white_row = 0;
        black_row = 0;

        for (auto hex : line) {
            if (board[hex] == WHITE_SYMBOL) {
                white_row++;
                black_row = 0;
            } else if (board[hex] == BLACK_SYMBOL) {
                white_row = 0;
                black_row++;
            } else if (board[hex] == EMPTY_PLACE_SYMBOL) {
                white_row = 0;
                black_row = 0;
            }

            if (white_row >= killing_number || black_row >= killing_number) {
                incorrect_rows++;
                break;
            }

        }
    }
    if (incorrect_rows == 0)
        return true;
    else if (incorrect_rows == 2)
        std::cout << "ERROR_FOUND_1_ROW_OF_LENGTH_K" << std::endl;
    else
        std::cout << "ERROR_FOUND_" << incorrect_rows/2 << "_ROWS_OF_LENGTH_K" << std::endl;
    return false;
}


std::vector<Hex> GIPF::read_move() {
    std::string raw_command;
    std::cin >> raw_command;

    std::string dot_position = raw_command.substr(0, 2);
    std::string entry_position = raw_command.substr(3, 2);
    char dash = raw_command[2];

    if (dot_position.length() == 2 && entry_position.length() == 2 && dash == '-') {
        Hex dot = translate(dot_position);
        Hex entry = translate(entry_position);
        if (dot == EMPTY_HEX) {
            std::cout << "BAD_MOVE_" << dot_position << "_IS_WRONG_INDEX" << std::endl;
            return std::vector({EMPTY_HEX, EMPTY_HEX});
        }
        if (entry == EMPTY_HEX) {
            std::cout << "BAD_MOVE_" << entry_position << "_IS_WRONG_INDEX" << std::endl;
            return std::vector({EMPTY_HEX, EMPTY_HEX});
        }

        std::vector move = {dot, entry};

        std::string details;
        std::getline(std::cin, details);
        if (details.length() > 0) {
            if (read_move_details(details, move)) {
//                do debugowania
//                for (auto hex : move) {
//                    std::cout << (char) board[hex] << std::endl;
//                }
                return move;
            } else {
                //std::cout << BAD_COMMAND_ERROR << "=" << details << "|" << std::endl;
                return std::vector({EMPTY_HEX, EMPTY_HEX});
            }
        } else {
            return move;
        }
    }

    //std::cout << BAD_COMMAND_FORMAT_ERROR << "dot=" << dot_position << "| dash=" << (char) dash << "| entry=" << entry_position << "|" << std::endl;
    return std::vector({EMPTY_HEX, EMPTY_HEX});
}

bool GIPF::read_move_details(const std::string& details, std::vector<Hex>& move) {
    std::vector<std::string> fragments = split_string(details);

    if(fragments.empty()) {
        return true;
    }

    if (fragments[0].size() != 2) {
        return false;
    }

    if(fragments[0] == "b:" || fragments[0] == "w:") {
        //std::cout << fragments[0] << std::endl;
        auto it = fragments.begin() + 1;
        while (it != fragments.end()) {
            Hex hex = translate(*it);
            if (hex == EMPTY_HEX) {
                std::cout << "BAD_MOVE_" << *it << "_IS_WRONG_INDEX" << std::endl;
                return false;
            } else {
                if (board[hex] != DOT_SYMBOL) {
                    move.push_back(hex);
                } else {
                    return false;
                }
            }
            it++;
        }
        return true;
    }

    return false;
}

void GIPF::make_move(std::vector<Hex> move) {
    if (!is_valid_move_basic(move)) {
        return;
    }

    int dir = move[0].neighbour_direction(move[1]);
    if (dir < 0 || dir >= HEX_DIRECTIONS_COUNT) {
        std::cout << "UNKNOWN_MOVE_DIRECTION" << std::endl;
        return;
    }

    auto line = board.get_fullline(move[0], dir);
    if (!board.line_has_empty_hex(line)) {
        std::cout << "BAD_MOVE_ROW_IS_FULL" << std::endl;
        return;
    }

    std::cout << "MOVE_COMMITTED" << std::endl;
    board.push_line(line, current_player);
    if (current_player == WHITE_SYMBOL)
        white_reserve--;
    else
        black_reserve--;

    next_player = (next_player == WHITE_SYMBOL) ? BLACK_SYMBOL : WHITE_SYMBOL;
    current_player = (current_player == WHITE_SYMBOL) ? BLACK_SYMBOL : WHITE_SYMBOL;
}

bool GIPF::is_valid_move_basic(std::vector<Hex> move) {
    if (move[0] == EMPTY_HEX || move[1] == EMPTY_HEX) {
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
    evaluate_turn(move);
}


bool GIPF::mark_target_line(const std::vector<Hex>& line) {
    std::cout << "marking line " << translate(line.front()) << "-->" << translate(line.back()) << std::endl;
    if (line.size() < killing_number) {
        std::cout << "\ttoo short line "<< line.size() << std::endl;
        return false;
    }

    int white_line = 0;
    int black_line = 0;
    bool col = false;
    std::vector<Hex> target_line;
    for (auto hex : line) {
        if ((white_line >= killing_number || black_line >= killing_number) && board[hex] == EMPTY_PLACE_SYMBOL) {
            std::cout << "\t\tbreaking at EMPTY after killing_number found" << std::endl;
            break;
        }

        if (board[hex] == WHITE_SYMBOL || board[hex] == WHITE_TARGET_OF_BLACK_SYMBOL
            || board[hex] == WHITE_TARGET_OF_WHITE_SYMBOL) {
            target_line.push_back(hex);
            if (black_line < killing_number) {
                white_line++;
                if (black_line) black_line = 0;
            }
        } else if (board[hex] == BLACK_SYMBOL || board[hex] == BLACK_TARGET_OF_BLACK_SYMBOL
                   || board[hex] == BLACK_TARGET_OF_WHITE_SYMBOL) {
            target_line.push_back(hex);
            if (white_line < killing_number) {
                black_line++;
                if (white_line) white_line = 0;
            }
        } else if (board[hex] == EMPTY_PLACE_SYMBOL) {
            if (!target_line.empty()) target_line.clear();
            if (white_line < killing_number) white_line = 0;
            if (black_line < killing_number) black_line = 0;
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
        std::cout << "\tNOT killing line b:" << black_line << " w:" << white_line << " t:" << target_line.size() << std::endl;
        return false;
    }

    if (col) {
        std::cout << "\t\tKOLIZJA TUTAJ" << std::endl;
//        for (auto x : line) {
//            std::cout << "\t" << translate(x) << " ";
//        }
//        std::cout << "\n";
        return true;
    }

   // std::cout << "\tkilling line == " << translate(line.front()) << "-->" << translate(line.back()) << std::endl;

    if (white_line >= killing_number) {
        for (auto hex : target_line) {
            if (board[hex] == BLACK_SYMBOL) {
                board.set(hex, BLACK_TARGET_OF_WHITE_SYMBOL);
            } else if (board[hex] == WHITE_SYMBOL) {
                board.set(hex, WHITE_TARGET_OF_WHITE_SYMBOL);
            } else if (board[hex] == EMPTY_PLACE_SYMBOL) {
                //board.set(hex, WHITE_TARGET_OF_WHITE_SYMBOL);
                std::cout << "\terror empty white target at " << translate(hex) << std::endl;
            } else {
                std::cout << "\terror white target at " << translate(hex) << " char = " << board[hex] << std::endl;
            }
        }
    } else if (black_line >= killing_number) {
        for (auto hex : target_line) {
            if (board[hex] == BLACK_SYMBOL) {
                board.set(hex, BLACK_TARGET_OF_BLACK_SYMBOL);
            } else if (board[hex] == WHITE_SYMBOL) {
                board.set(hex, WHITE_TARGET_OF_BLACK_SYMBOL);
            } else if (board[hex] == EMPTY_PLACE_SYMBOL) {
                //board.set(hex, BLACK_TARGET_OF_BLACK_SYMBOL);
                std::cout << "\terror empty black target at " << translate(hex) << std::endl;
            } else {
                std::cout << "\terror black target at " << translate(hex) << " char = " << board[hex] << std::endl;
            }
        }
    }

//    for (auto hex : line) {
//        if (board[hex] == WHITE_SYMBOL || board[hex] == BLACK_SYMBOL || board[hex] == EMPTY_PLACE_SYMBOL) {     //ie is not already part of target line
//            //std::cout << "marking " << translate(hex) << " --> ";
//            if (white_line >= killing_number) {
//                int wm = 0;
//                if (board[hex] == BLACK_SYMBOL) {
//                    //std::cout << "b->w at " << translate(hex) << std::endl;
//                    board.set(hex, BLACK_TARGET_OF_WHITE_SYMBOL);
//                } else if (board[hex] == WHITE_SYMBOL) {
//                    //std::cout << "w->w at " << translate(hex) << std::endl;
//                    board.set(hex, WHITE_TARGET_OF_WHITE_SYMBOL);
//                } else if (board[hex] == EMPTY_PLACE_SYMBOL) {
//                    //board.set(hex, EMPTY_TARGET_SYMBOL);
//                    //std::cout << "_->_ at " << translate(hex) << std::endl;
//                    //std::cout << "breaking on _ at " << translate(hex) << std::endl;
//                    //break;
//                } else {
//                    //std::cout << "error char at " << translate(hex) << " == " << board[hex] << std::endl;
//                }
//            } else if (black_line >= killing_number) {
//                if (board[hex] == WHITE_SYMBOL) {
//                    //std::cout << "w->b at " << translate(hex) << std::endl;
//                    board.set(hex, WHITE_TARGET_OF_BLACK_SYMBOL);
//                } else if (board[hex] == BLACK_SYMBOL) {
//                    //std::cout << "b->b at " << translate(hex) << std::endl;
//                    board.set(hex, BLACK_TARGET_OF_BLACK_SYMBOL);
//                } else if (board[hex] == EMPTY_PLACE_SYMBOL) {
//                    //std::cout << "breaking on _ at " << translate(hex) << std::endl;
//                    //break;
//                }
//            }
//
//        } else {
//            //przecięcie
//           // std::cout << "kolizja " << translate(hex) << std::endl;
//            return true;
//        }
//    }
    return false;
}

void GIPF::evaluate_turn(const std::vector<Hex> &last_move) {
    bool special_move = last_move.size() > 2;
    //board.print_cont();
    auto dotlines = board.get_dotlines();
    if (dotlines.empty()) {
        std::cout << "no dotlines" << std::endl;
        return;
    }
//    for (auto line : dotlines) {
//        std::cout << "line: " << translate(line.front()) << " --> " << translate(line.back()) << std::endl;
//    }

    //std::cout << "dotlines: " << dotlines.size() << std::endl;
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
        //std::cout << "checking line " << i << std::endl;
        i++;
    }
    //std::cout << "v=" << v << "_of_i=" << i << "_" << std::endl;

//    std::cout << "\n\tcoords" << std::endl;
//    print_coords();
//    std::cout << "\n\ttargets acquired" << std::endl;
//    board.print_gipf();
//    std::cout << "\n\ttargets killed" << std::endl;
    kill_targets();
//    board.print_gipf();
//    std::cout << "\n\ttargets cleared" << std::endl;
    board.clear_board();
    //board.print_gipf();
}

void GIPF::kill_targets() {
    for (const auto& kv : board.map) {
        if (kv.second == WHITE_TARGET_OF_WHITE_SYMBOL || kv.second == WHITE_TARGET_OF_BLACK_SYMBOL
            || kv.second == BLACK_TARGET_OF_WHITE_SYMBOL || kv.second == BLACK_TARGET_OF_BLACK_SYMBOL) {

            if (kv.second == WHITE_TARGET_OF_WHITE_SYMBOL) {
                white_reserve++;
            } else if (kv.second == BLACK_TARGET_OF_WHITE_SYMBOL) {
                prisoners_of_white++;
            } else if (kv.second == WHITE_TARGET_OF_BLACK_SYMBOL) {
                prisoners_of_black++;
            } else if (kv.second == BLACK_TARGET_OF_BLACK_SYMBOL) {
                black_reserve++;
            }

            board.set(kv.first, EMPTY_PLACE_SYMBOL);
        }
    }
}


std::string GIPF::translate(const Hex &hex) const {
    try {
        return hex_coords.at(hex);
    } catch (std::out_of_range& e) {
        return "XXX";
    }
}

Hex GIPF::translate(const std::string &coords) const {
    try {
        return coords_hex.at(coords);
    } catch (std::out_of_range &e) {
        return EMPTY_HEX;
    }
}


void GIPF::print_game_state() const {
    std::cout << size << " " << killing_number << " " << base_white_pieces << " " << base_black_pieces << std::endl;
    std::cout << white_reserve << " " << black_reserve << " " << current_player << std::endl;
    board.print_gipf();
}

void GIPF::print_coords() const {
    int n = board.playing_field_size;

    for (int q = -n; q <= n; q++) {
        int r1 = std::max(-n, -q - n);
        int r2 = std::min(n, -q + n);

        for (int i = -n; i < n - r2 + r1; i++) {
            std::cout << "  ";
        }

        for (int r = r1; r <= r2; r++) {
            Hex rh = rotate_left(Hex(q, r));
            if (board.map.at(rh) == DOT_SYMBOL || board.map.at(rh) == USED_DOT_SYMBOL)
                std::cout << translate(rh) << "  ";
            else
                std::cout << translate(rh) << "  ";
        }
        std::cout << std::endl;
    }
}




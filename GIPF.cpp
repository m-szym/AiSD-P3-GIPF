//
// Created by M Sz on 17/05/2023.
//

#include "GIPF.h"

GIPF::GIPF(int size, int killing_number, int white_pieces, int black_pieces, int white_reserve, int black_reserve, char starting_player)
        : hex_coords{}, coords_hex{}, board(size), size(size), killing_number(killing_number), base_white_pieces(white_pieces),
          base_black_pieces(black_pieces), prisoners_of_white(0), prisoners_of_black(0), white_reserve(white_reserve), black_reserve(black_reserve),
          state(IN_PROGRESS)
{
    current_player = starting_player;
    next_player = (starting_player == WHITE_SYMBOL) ? BLACK_SYMBOL : WHITE_SYMBOL;

    board.construct_map(hex_coords, coords_hex);
    board.read_map();

    check_map();
}

GIPF::GIPF(int params[], char starting_player, std::vector<char> flat_board, bool &good_state)
        : board(params[0]), state(IN_PROGRESS) {
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

GIPF::GIPF(const GIPF &other)
        : board(other.board), size(other.size), killing_number(other.killing_number), base_white_pieces(other.base_white_pieces),
          base_black_pieces(other.base_black_pieces), prisoners_of_white(other.prisoners_of_white), prisoners_of_black(other.prisoners_of_black),
          white_reserve(other.white_reserve), black_reserve(other.black_reserve), current_player(other.current_player), next_player(other.next_player),
          coords_hex(other.coords_hex), hex_coords(other.hex_coords), state(other.state)
{}

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
    else if (incorrect_rows == 1)
        std::cout << "ERROR_FOUND_1_ROW_OF_LENGTH_K" << std::endl;
    else
        std::cout << "ERROR_FOUND_" << incorrect_rows << "_ROWS_OF_LENGTH_K" << std::endl;
    return false;
}


std::pair<std::vector<Hex>, char> GIPF::read_move() {
    std::string raw_command;
    std::cin >> raw_command;

    char delimiter = '-';
    std::string dot_position = raw_command.substr(0, raw_command.find(delimiter));
    std::string entry_position = raw_command.substr(raw_command.find(delimiter) + 1, raw_command.length());
    //char dash = raw_command[2];

    //if (dot_position.length() == 2 && entry_position.length() == 2 && dash == '-') {
        Hex dot = translate(dot_position);
        Hex entry = translate(entry_position);
        if (dot == EMPTY_HEX) {
            std::cout << "BAD_MOVE_" << dot_position << "_IS_WRONG_INDEX" << std::endl;
            state = "BAD_MOVE ";
            state += current_player;
            state += " " + raw_command;
            return {{EMPTY_HEX}, DOT_SYMBOL};
        }
        if (entry == EMPTY_HEX) {
            std::cout << "BAD_MOVE_" << entry_position << "_IS_WRONG_INDEX" << std::endl;
            state = std::string("BAD_MOVE " + std::to_string((char)current_player) + raw_command);
            state = "BAD_MOVE ";
            state += current_player;
            state += " " + raw_command;
            return {{EMPTY_HEX}, DOT_SYMBOL};
        }

        std::vector move = {dot, entry};
        char player = DOT_SYMBOL;

        std::string details;
        std::getline(std::cin, details);
        if (details.length() > 0) {
            if (read_move_details(details, move, player)) {
                if(move.size() == 4) {
                    return {move, player};
                }
            } else {
                std::cout << BAD_COMMAND_ERROR << " details ==" << details << "|" << std::endl;
                state = "BAD_MOVE ";
                state += current_player;
                state += " " + raw_command;
                state += " " + details;
                return {{EMPTY_HEX}, DOT_SYMBOL};
            }
        } else {
            return {move, player};
        }
    //}

    //std::cout << BAD_COMMAND_FORMAT_ERROR << "dot=" << dot_position << "| dash=" << (char) dash << "| entry=" << entry_position << "|" << std::endl;
    state = "BAD_MOVE ";
    state += current_player;
    state += " " + raw_command;
    return {{EMPTY_HEX}, DOT_SYMBOL};
}

bool GIPF::read_move_details(const std::string &details, std::vector<Hex> &move, char &killer) {
    std::vector<std::string> fragments = split_string(details);

    if(fragments.empty()) {
        return true;
    }

    if(fragments.size() != 3) {
        state = "BAD_MOVE ";
        state += current_player;
        state += " " + translate(move[0]) + "-" + translate(move[1]) + " " + details;
        return false;
    }

    std::vector<Hex> tmp;
    if(fragments[0] == "b:" || fragments[0] == "w:") {
        killer = (fragments[0] == "w:") ? WHITE_SYMBOL : BLACK_SYMBOL;

        auto it = fragments.begin() + 1;
        while (it != fragments.end()) {
            Hex hex = translate(*it);
            if (hex == EMPTY_HEX) {
                std::cout << "BAD_MOVE_" << *it << "_IS_WRONG_INDEX" << std::endl;
                state = "BAD_MOVE ";
                state += current_player;
                state += " " + translate(move[0]) + "-" + translate(move[1]) + " " + details;
                return false;
            } else {
                if (board[hex] != DOT_SYMBOL) {
                    tmp.push_back(hex);
                } else {
                    state = "BAD_MOVE ";
                    state += current_player;
                    state += " " + translate(move[0]) + "-" + translate(move[1]) + " " + details;
                    return false;
                }
            }
            it++;
        }
        move.insert(move.end(), tmp.begin(), tmp.end());
        return true;
    } else {
        state = "BAD_MOVE ";
        state += current_player;
        state += " " + translate(move[0]) + "-" + translate(move[1]) + " " + details;
        return false;
    }
}

bool GIPF::make_move(const std::pair<std::vector<Hex>, char>& move) {
    if (!is_valid_move_basic(move.first)) {
        return false;
    }

    int dir = move.first[0].neighbour_direction(move.first[1]);
    if (dir < 0 || dir >= HEX_DIRECTIONS_COUNT) {
        std::cout << "UNKNOWN_MOVE_DIRECTION" << std::endl;
        return false;
    }

    auto line = board.get_fullline(move.first[0], dir);
    if (!board.line_has_empty_hex(line)) {
        std::cout << "BAD_MOVE_ROW_IS_FULL" << std::endl;
        return false;
    }

    std::cout << "MOVE_COMMITTED" << std::endl;
    board.push_line(line, current_player);
    if (current_player == WHITE_SYMBOL)
        white_reserve--;
    else
        black_reserve--;

    next_player = (next_player == WHITE_SYMBOL) ? BLACK_SYMBOL : WHITE_SYMBOL;
    current_player = (current_player == WHITE_SYMBOL) ? BLACK_SYMBOL : WHITE_SYMBOL;
    return true;
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

bool GIPF::simple_move() {
    auto move = read_move();
    if(!make_move(move))
        return false;
    return evaluate_turn(move);
}


bool GIPF::mark_target_line(const std::vector<Hex>& line) {
    //std::cout << "marking line " << translate(line.front()) << "-->" << translate(line.back()) << std::endl;
    if (line.size() < killing_number) {
        //std::cout << "\ttoo short line "<< line.size() << std::endl;
        return false;
    }

    int white_line = 0;
    int black_line = 0;
    bool col = false;
    std::vector<Hex> target_line;
    for (auto hex : line) {
        if ((white_line >= killing_number || black_line >= killing_number) && board[hex] == EMPTY_PLACE_SYMBOL) {
            //std::cout << "\t\tbreaking at EMPTY after killing_number found" << std::endl;
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
        //std::cout << "\tNOT killing line b:" << black_line << " w:" << white_line << " t:" << target_line.size() << std::endl;
        return false;
    }

    if (col) {
        //std::cout << "\t\tKOLIZJA TUTAJ" << std::endl;
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
                //std::cout << "\terror empty white target at " << translate(hex) << std::endl;
            } else {
                //std::cout << "\terror white target at " << translate(hex) << " char = " << board[hex] << std::endl;
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
                //std::cout << "\terror empty black target at " << translate(hex) << std::endl;
            } else {
                //std::cout << "\terror black target at " << translate(hex) << " char = " << board[hex] << std::endl;
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

bool GIPF::evaluate_turn(const std::pair<std::vector<Hex>, char> &last_move) {
    auto move_hexes = last_move.first;
    char player = last_move.second;
    bool special_move = ((move_hexes.size() == 4) && (player == WHITE_SYMBOL || player == BLACK_SYMBOL));

    auto dotlines = board.get_dotlines();
    if (dotlines.empty()) {
        std::cout << "no dotlines" << std::endl;
        return false;
    }
//    for (auto line : dotlines) {
//        std::cout << "line: " << translate(line.front()) << " --> " << translate(line.back()) << std::endl;
//    }

    //std::cout << "dotlines: " << dotlines.size() << std::endl;
    if (special_move) {
        //std::cout << "special move" << std::endl;
        Hex start = move_hexes[2];
        Hex end = move_hexes[3];
        //std::cout << "start = " << translate(start) << " end = " << translate(end) << std::endl;
        auto special_line = board.simple_get_line(start, end);
        print_line(special_line);
        auto it = special_line.begin();

        while (it != special_line.end()) {
            if (board[*it] == WHITE_SYMBOL) {
                if (player == WHITE_SYMBOL) {
                    //board.set(*it, WHITE_TARGET_OF_WHITE_SYMBOL);
                    board.set(*it, EMPTY_PLACE_SYMBOL);
                    white_reserve++;
                } else if (player == BLACK_SYMBOL) {
                    board.set(*it, EMPTY_PLACE_SYMBOL);
                    prisoners_of_white++;
                }
                    //board.set(*it, WHITE_TARGET_OF_BLACK_SYMBOL);

            } else if (board[*it] == BLACK_SYMBOL) {
                if (player == WHITE_SYMBOL) {
                    //board.set(*it, BLACK_TARGET_OF_WHITE_SYMBOL);
                    board.set(*it, EMPTY_PLACE_SYMBOL);
                    prisoners_of_black++;
                }
                else if (player == BLACK_SYMBOL) {
                    //board.set(*it, BLACK_TARGET_OF_BLACK_SYMBOL);
                    board.set(*it, EMPTY_PLACE_SYMBOL);
                    black_reserve++;
                }
            }
//            } else if (board[*it] == EMPTY_PLACE_SYMBOL) {
//                board.set(*it, '3');
//            }
            it++;
        }
    } /*else {
        std::cout << "special move ERROR" << "p: " << player << " s: " << move_hexes.size();
        std::cout << " start: " << translate(move_hexes[2]) << " end: " << translate(move_hexes[3]) << std::endl;
        return false;
    }*/

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
            return false;   //TODO czy to jest ok?
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
    return true;
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

void GIPF::print_line(const std::vector<Hex> &line) const {
    std::cout << "line: " << translate(line.front()) << " --> " << translate(line.back()) << std::endl;
    std::cout << "\t";
    for (const Hex& hex : line) {
        std::cout << translate(hex);
        if (hex != line.back())
            std::cout << " --> ";
    }
    std::cout << std::endl;
}

std::string GIPF::check_state() {
    if(white_reserve <= 0)
        state = BLACK_WIN;
    else if(black_reserve <= 0)
        state = WHITE_WIN;
    else {
        int empty = 0;
        for (auto kv : board.map) {
            if (kv.second == EMPTY_PLACE_SYMBOL)
                empty++;
        }
        if (empty == 0)
            state = DEAD_LOCK + current_player;
        else
            state = IN_PROGRESS;
    }

    return state;
}






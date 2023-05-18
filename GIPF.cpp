//
// Created by M Sz on 17/05/2023.
//

#include "GIPF.h"

GIPF::GIPF(int size, int killing_number, int white_pieces, int black_pieces)
    : board(size), size(size), killing_number(killing_number), base_white_pieces(white_pieces),
      base_black_pieces(black_pieces), white_captured(0), black_captured(0), white_reserve(0), black_reserve(0),
      next_player(BLACK_SYMBOL), current_player(WHITE_SYMBOL)
{
    //board.read();
}

GIPF::GIPF(std::vector<int> params)
    : board(params[0]), size(params[0]), killing_number(params[1]), base_white_pieces(params[2]),
      base_black_pieces(params[3]), white_captured(0), black_captured(0), white_reserve(0), black_reserve(0),
      next_player(BLACK_SYMBOL), current_player(WHITE_SYMBOL)
{
    //board.read();
}

std::pair<Hex, Hex> GIPF::read_move() {
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
                    Hex dot = board.key_map.at(dot_position);
                    dot.print();
                    std::cout << (char) board[dot] << std::endl;
                    try {
                        Hex entry = board.key_map.at(entry_position);
                        entry.print();
                        std::cout << (char) board[entry] << std::endl;

                        return std::make_pair(dot, entry);
                    } catch (std::out_of_range& e) {
                        std::cout << BAD_COORDINATES_ERROR << std::endl;
                        return std::make_pair(EMPTY_HEX, EMPTY_HEX);
                    }
                } catch (std::out_of_range& e) {
                    std::cout << BAD_COORDINATES_ERROR << std::endl;
                    return std::make_pair(EMPTY_HEX, EMPTY_HEX);
                }
            }
        }
    }

    std::cout << BAD_COMMAND_FORMAT_ERROR << std::endl;
    return std::make_pair(EMPTY_HEX, EMPTY_HEX);
}

void GIPF::make_move(std::pair<Hex, Hex> move) {
    if (move.first == EMPTY_HEX || move.second == EMPTY_HEX) {
        return;
    }

    board.set(move.first, current_player);

    if (is_valid_move_basic(move)) {
        int dir = move.first.neighbour_direction(move.second);
        auto path = board.get_line(move.first, dir);
        if (board.line_has_empty_hex(path)) {
            board.push_line(path, current_player);
        }
    } else {
        std::cout << BAD_MOVE_ERROR << std::endl;
        return;
    }

    board.set(move.first, DOT_SYMBOL);
    next_player = (next_player == WHITE_SYMBOL) ? BLACK_SYMBOL : WHITE_SYMBOL;

}

bool GIPF::is_valid_move_basic(std::pair<Hex, Hex> move) {
    if (move.first == EMPTY_HEX || move.second == EMPTY_HEX) {
        return false;
    }

    if (move.first.distance(move.second) != 1) {
        return false;
    }



    return true;
}

void GIPF::simple_move() {
    std::pair<Hex, Hex> move = read_move();
    make_move(move);
}
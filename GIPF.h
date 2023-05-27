//
// Created by M Sz on 17/05/2023.
//

#ifndef GIPF_GIPF_H
#define GIPF_GIPF_H

#include <sstream>
#include <iterator>
#include <unordered_map>
#include <utility>

#include "Board.h"
#include "other.h"

class GIPF {
public:
    Board board;

    int size;
    int killing_number;
    int base_white_pieces;
    int base_black_pieces;

    int prisoners_of_white;
    int prisoners_of_black;
    int white_reserve;
    int black_reserve;

    char current_player;
    char next_player;

    std::unordered_map<std::string, Hex> coords_hex;
    std::unordered_map<Hex, std::string> hex_coords;

    std::string state;


    GIPF(int size, int killing_number, int white_pieces, int black_pieces, int white_reserve, int black_reserve, char starting_player);
    GIPF(int params[], char starting_player, std::vector<char> flat_board, bool &good_state);

    GIPF(const GIPF& other);       // copy constructor

    bool check_map();

    std::string check_state();

    void print_game_state() const;
    void print_coords() const;

    void print_line(const std::vector<Hex>& line) const;

    std::pair<std::vector<Hex>, char> read_move();
    bool read_move_details(const std::string &details, std::vector<Hex> &move, char &killer);

    bool is_valid_move_basic(std::vector<Hex> move);
    bool make_move(const std::pair<std::vector<Hex>, char>& move);
    bool simple_move();

    bool mark_target_line(const std::vector<Hex>& line);
    bool evaluate_turn(const std::pair<std::vector<Hex>, char> &last_move);
    void kill_targets();

    char add_piece(const std::string& coords, char piece) { return board.set(translate(coords), piece); }
    char add_piece(const Hex& hex, char piece) { return board.set(hex, piece); }

    std::string translate(const Hex& hex) const;
    Hex translate(const std::string& coords) const;
};


#endif //GIPF_GIPF_H

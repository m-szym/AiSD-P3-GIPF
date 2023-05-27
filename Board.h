//
// Created by M Sz on 15/05/2023.
//

#ifndef GIPF_BOARD_H
#define GIPF_BOARD_H

#include <string>
#include <unordered_map>
#include <algorithm>

#include "Hex.h"
#include "constants.h"

class Board {

public:
    int real_size;
    int playing_field_size;
    std::unordered_map<Hex, char> map;

    explicit Board(int size);
    Board(const Board& other);  //copy constructor

    char operator[](const Hex& hex) const;
    bool is_dot(Hex hex) const;

    void print_gipf () const;
    void print_xN() const;
    void print_cont() const;

    void read_map();
    void new_read_map(std::vector<char> flat_board);
    void find_dots();
    void construct_map(std::unordered_map<Hex, std::string> &hex_coords,
                       std::unordered_map<std::string, Hex> &coords_hex);

    std::vector<Hex> get_fullline(Hex starting_hex, int direction);
    std::vector<Hex> simple_get_line(Hex starting_hex, Hex ending_hex);
    std::vector<std::vector<Hex>> get_dotlines();
    bool line_was_visited(const std::vector<Hex> &line) const;
    bool line_has_empty_hex(const std::vector<Hex>& line);

    void push_line(std::vector<Hex> line, char value);

    char set(Hex hex, char value);

    void clear_board();
};




#endif //GIPF_BOARD_H

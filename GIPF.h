//
// Created by M Sz on 17/05/2023.
//

#ifndef GIPF_GIPF_H
#define GIPF_GIPF_H


#include "Board.h"

class GIPF {
public:
    Board board;

    int size;
    int killing_number;
    int base_white_pieces;
    int base_black_pieces;

    int white_captured;
    int black_captured;
    int white_reserve;
    int black_reserve;

    char current_player;
    char next_player;

public:
    GIPF(int size, int killing_number, int white_pieces, int black_pieces);
    GIPF(std::vector<int> params);

    std::pair<Hex, Hex> read_move();
    bool is_valid_move_basic(std::pair<Hex, Hex> move);
    void make_move(std::pair<Hex, Hex> move);
    void simple_move();

    void kill_line(std::vector<Hex> line);


};


#endif //GIPF_GIPF_H

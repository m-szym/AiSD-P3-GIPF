//
// Created by M Sz on 15/05/2023.
//

#ifndef GIPF_BOARD_H
#define GIPF_BOARD_H


#include "Hex.h"
#include "constants.h"

#include <string>
#include <unordered_map>

class Board {
public:
    int real_size;
    int playing_field_size;
    Hex **board;
    std::unordered_map<Hex, int> map;
    std::unordered_map<std::string, Hex> key_map;

public:
    explicit Board(int size);

    int operator[](const Hex& hex) const;   //<- do poprawy

    void print() const;

    void print_xN() const;

    void print_cont() const;

    void naive_read();  //<- działa, wczytuje dobrze (wsp. gipfowe)

    void print_rot_l() const;   //<- działa, obraca w lewo (wyświetlanie jak gipf for 1)
};


#endif //GIPF_BOARD_H

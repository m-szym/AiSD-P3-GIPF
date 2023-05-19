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
private:
    const std::string SPACER = " ";
    const int GIPF_INDEX_START_INT = 1;
    const char GIPF_INDEX_START_CHAR = 'A';

public:
    int real_size;
    int playing_field_size;
    Hex **board;
    std::unordered_map<Hex, char> map;
    std::unordered_map<std::string, Hex> key_map;

public:
    explicit Board(int size);

    int operator[](const Hex& hex) const;   //<- do poprawy dalej
    int operator[](const std::string& gf1_coords) const;   //<- do poprawy dalej

    //void print() const;
    void print_gipf () const;
    void print_coords() const;
    void print_coords_gipf() const;

    void print() const;

    void print_xN() const;

    void print_cont() const;

    void read_map();  //<- działa, wczytuje dobrze (wsp. gipfowe) - według stosa

    void find_dots();   //TODO

    std::vector<Hex> get_fullline(Hex starting_hex, int direction);     //każdy Hex w linii na pewno znajduje się na planszy
                                                                        //można obiecać, że nie będzie out_of_range
                                                                        //nie bierze kropek

    std::vector<Hex> get_wline(Hex starting_hex, int direction);        //jak fullline, ale do pierwszego wolnego miejsca / kropki

    bool line_has_empty_hex(const std::vector<Hex>& line);
    void push_line(std::vector<Hex> line, char value);


    char set(Hex hex, char value);
    char set(const std::string& gf1_coords, char value);

    bool is_dot(Hex hex) const;                             //obsługa wyjątków
    bool is_dot(const std::string& gf1_coords) const;       //obsługa wyjątków


};




#endif //GIPF_BOARD_H

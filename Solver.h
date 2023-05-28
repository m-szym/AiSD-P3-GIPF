//
// Created by marek on 28.05.2023.
//

#ifndef GIPF_SOLVER_H
#define GIPF_SOLVER_H

#include "AllTree.h"
#include "GIPF.h"

class Solver {
public:
    AllTree<GIPF*> *root;

    explicit Solver(GIPF *og_game);
    ~Solver();


    std::string translate(Hex hex) const { return root->data->translate(hex); }
    Hex translate(const std::string& coords) const { return root->data->translate(coords); }

    //std::string translate_move(std::pair<Hex, Hex> move) const { return {translate(move.first) + "-" + translate(move.second)}; }
    std::string translate_move(Hex hex1, Hex hex2) const { return {translate(hex1) + "-" + translate(hex2)}; }
    std::string translate_simple_move(std::vector<Hex> move) const { return {translate(move[0]) + "-" + translate(move[1])}; }

    std::vector<std::string> translate_simple_moves(const std::vector<std::vector<Hex>> &moves) const;

    std::vector<std::vector<Hex>> get_all_possible_simple_moves();

    std::vector<std::vector<Hex>> choose_legal_simple_moves(const std::vector<std::vector<Hex>>& moves);
};


#endif //GIPF_SOLVER_H

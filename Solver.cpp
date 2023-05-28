//
// Created by marek on 28.05.2023.
//

#include "Solver.h"

Solver::Solver(GIPF *og_game) {
    root = new AllTree<GIPF*>(og_game);

}

Solver::~Solver() {
    delete root;
}

std::vector<std::vector<Hex>> Solver::get_all_possible_simple_moves() {
    auto moves = std::vector<std::vector<Hex>>();
    GIPF game = *root->data;

    for (auto kv : game.board.map) {
        if (kv.second == DOT_SYMBOL) {
            Hex dot = kv.first;
            for (int i = 0; i < HEX_DIRECTIONS_COUNT; ++i) {
                Hex entry = dot.hex_neighbour(i);
                if (game.board.map.find(entry) != game.board.map.end() && game.board.map[entry] != DOT_SYMBOL) {
                    moves.push_back({dot, entry});
                }

            }
        }
    }

    return moves;
}

std::vector<std::vector<Hex>> Solver::choose_legal_simple_moves(const std::vector<std::vector<Hex>>& moves) {
    auto legal_moves = std::vector<std::vector<Hex>>();
    GIPF game = *root->data;

    for (auto move : moves) {
        auto game_copy = GIPF(game);
        if(game_copy.simple_move(move)) legal_moves.push_back(move);
    }

    return legal_moves;
}

std::vector<std::string> Solver::translate_simple_moves(const std::vector<std::vector<Hex>> &moves) const {
    std::vector<std::string> translated_moves;
    translated_moves.reserve(moves.size());
    for (auto move : moves) {
        translated_moves.push_back(translate_simple_move(move));
    }
    return translated_moves;
}






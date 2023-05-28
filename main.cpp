#include <iostream>
#include <string>
#include <memory>

#include "Board.h"
#include "GIPF.h"
#include "other.h"
#include "Solver.h"

//
//           W _ _ B
//          _ _ _ _ _
//         _ _ _ _ _ _
//        B _ _ _ _ _ W
//         _ _ _ _ _ _
//          _ _ _ _ _
//           W _ _ B

/*
 *     N              NW SW
 *  NW * NE          N  *  S
 *  SW * SE           NE SE
 *     S
 */

/*
 *       SE = 0
 *       NE = 1               SW SS
 *       N  = 2              NW * SE
 *       NW = 3               NN NE
 *       SW = 4
 *       S  = 5
 */

int main() {
    std::string input;
    std::unique_ptr<GIPF> g = nullptr;

    while (std::cin) {
        if (std::cin.eof()) break;
        input = "";
        std::cin >> input;

        if (input == "exit")
            break;
        else if (input == LOAD_BOARD_COMMAND) {
            if (g != nullptr) g.reset();

            int int_params[6] = {0};
            char next_player = ' ';

            std::cin >> int_params[0] >> int_params[1] >> int_params[2];
            std::cin >> int_params[3] >> int_params[4] >> int_params[5] >> next_player;


            std::vector<char> fields;

            int whites = 0;
            int blacks = 0;
            while (!std::cin.eof()) {
                int n = std::cin.peek();
                if (n == '\n' || n == ' ') {
                    std::cin.get();
                    continue;
                } else if (n == EMPTY_PLACE_SYMBOL || n == WHITE_SYMBOL || n == BLACK_SYMBOL) {
                    if (n == WHITE_SYMBOL) whites++;
                    else if (n == BLACK_SYMBOL) blacks++;
                    fields.push_back((char) std::cin.get());
                } else
                    break;
            }

            int size = 0;
            int row = int_params[0];
            for (int j = row; j < ((2 * row) - 1); j++) {
                size += j;
            }
            size *= 2;
            size += (2 * row) - 1;
            if (fields.size() != size) {
                std::cout << INVALID_BOARD_ROW_ERROR << std::endl;
            } else {

                if (((int_params[2]) - whites) < int_params[4]) {
                    std::cout << INVALID_BOARD_WHITE_PIECES_ERROR << std::endl;
                } else if (int_params[3] - blacks < int_params[5]) {
                    std::cout << INVALID_BOARD_BLACK_PIECES_ERROR << std::endl;
                } else {
                    bool good_state = false;
                    g = std::make_unique<GIPF>(int_params, next_player, fields, good_state);

                    if (good_state)
                        std::cout << BOARD_LOAD_OK << std::endl;
                    else
                        g.reset();
                }
            }
            std::cout << std::endl;
        }
        else if (input == DO_MOVE_COMMAND) {
            if (g == nullptr) continue;
            if (g->state == WHITE_WIN || g->state == BLACK_WIN || g->state.contains(DEAD_LOCK)) {
                std::cout << "GAME_OVER " << g->state << std::endl;
                continue;
            }

            auto gcopy = std::make_unique<GIPF>(*g);

            if(!g->simple_move()) {
                gcopy->state = g->state;
                g.swap(gcopy);
            } else {
                g->check_state();
                std::cout << "MOVE_COMMITTED" << std::endl;
            }

            std::cout << std::endl;
        }
        else if (input == PRINT_GAME_COMMAND) {
            if (g == nullptr) continue;

            g->print_game_state();
            std::cout << std::endl;
        } else if (input == "debug") {
            /*
            std::cout << "NE:" << g->translate(g->translate("d4").hex_neighbour(NE)) << std::endl;
            std::cout << "NW:" << g->translate(g->translate("d4").hex_neighbour(NW)) << std::endl;
            std::cout << "SE:" << g->translate(g->translate("d4").hex_neighbour(SE)) << std::endl;
            std::cout << "SW:" << g->translate(g->translate("d4").hex_neighbour(SW)) << std::endl;
            std::cout << "N:" << g->translate(g->translate("d4").hex_neighbour(N)) << std::endl;
            std::cout << "S:" << g->translate(g->translate("d4").hex_neighbour(S)) << std::endl;
            g->board.simple_get_line(g->translate("d4"), g->translate("b4"));
            g->board.simple_get_line(g->translate("d4"), g->translate("d6"));
            g->board.simple_get_line(g->translate("d4"), g->translate("d2"));
            g->board.simple_get_line(g->translate("d4"), g->translate("b2"));
            g->board.simple_get_line(g->translate("d4"), g->translate("g2"));
            g->board.simple_get_line(g->translate("d4"), g->translate("h5"));
             */

            Solver s(new GIPF(*g));
            auto moves = s.get_all_possible_simple_moves();
            auto legal_moves = s.choose_legal_simple_moves(moves);
            auto translated_moves = s.translate_simple_moves(legal_moves);

            std::cout << moves.size() << " possible moves" << std::endl;
            std::cout << legal_moves.size() << " legal moves" << std::endl;
            int i = 0;
            for (auto &move : translated_moves) {
                std::cout << "\tMove [" << i << "]" << " " << move << std::endl;
                i++;
            }
        } else if (input == PRINT_STATE_COMMAND) {
            if (g == nullptr) continue;

            std::cout << g->state << std::endl;
            std::cout << std::endl;
        }
    }

    return 0;
}




#include <iostream>
#include <string>
#include <memory>

#include "Board.h"
#include "GIPF.h"
#include "other.h"

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

int main() {
   //Board b(4);
//
    //b.read_map();

    //b.map[b.key_map.at("B5")] = 'b';
//    std::cout << "\n\n";
//    b.print();
//    std::cout << "\n\n";
//    b.print_xN();
//    std::cout << "\n\n";
//    b.print_cont();
//    std::cout << "\n\n";
//    b.print_gipf();
//    std::cout << "\n\n";
//    b.print_coords_gipf();
//    std::cout << "\n\n";
//
//    for (int a = 'A'; a <= 'I'; a++) {
//        for (int i = 1; i <= 9; i++) {
//            try {
//                std::string s = std::to_string(i) + std::string(1, (char) a);
//                std::cout << s << " == " << (char) b.map.at(b.key_map.at(s)) << "\n";
//            } catch (std::out_of_range& e) {
//                std::cout << "out of range\n";
//                continue;
//            }
//        }
//        std::cout << std::endl;
//    }

//    Hex a1(1,1);
//    Hex a2(2,2);
//    a1.print();
//    a2.print();
//    std::swap(a1, a2);
//    a1.print();
//    a2.print();

//    b.set("2B", 'b');
//    b.set("3C", 'b');
//    b.set("4D", 'b');
//    b.set("5E", 'b');
//    b.set("5F", 'b');
//    b.set("5G", 'b');
//
//    b.print_cont();
//    //b.print_xN();
//    auto l = b.get_fullline(b.key_map.at("2B"), DIRS::SE);
//    for (auto& x : l) {
//        //x.print();
//    }
//    if (b.line_has_empty_hex(l))
//        b.push_line(l, 'X');
//
//    b.print_cont();


//    b.set(Hex(0,0).hex_neighbour(DIRS::SE), '0');
//    b.set(Hex(0,0).hex_neighbour(DIRS::NE), '1');
//    b.set(Hex(0,0).hex_neighbour(DIRS::N), '2');
//    b.set(Hex(0,0).hex_neighbour(DIRS::NW), '3');
//    b.set(Hex(0,0).hex_neighbour(DIRS::SW), '4');
//    b.set(Hex(0,0).hex_neighbour(DIRS::S), '5');
//
//    b.print_cont();

//    GIPF g(4,4,4,4);
//    //g.board.read_map();
//    g.add_piece("2B", 'W');
//    g.add_piece("3C", 'W');
//    g.add_piece("4D", 'W');
//    g.add_piece("5D", 'W');
//    g.add_piece("5C", 'W');
//    g.add_piece("5B", 'W');
//
//    g.board.print_cont();
//    g.board.print_xN();
//
//    //g.read_move();
//    while (true) {
//        g.simple_move();
//        g.board.print_cont();
//    }

    std::string input;
    std::unique_ptr<GIPF> g = nullptr;
    //std::cout << "\n";
    while (std::cin) {
        if (std::cin.eof()) break;
        input = "";
        std::cin >> input;

        if (input == "exit")
            break;
        else if (input == LOAD_BOARD_COMMAND) {
            int init[6] = {0};
            int size = 0;
            int killig_number = 0;
            int white_pieces = 0;
            int black_pieces = 0;
            int white_reserve = 0;
            int black_reserve = 0;
            char next_player = ' ';
            //std::cin >> size >> killig_number >> white_pieces >> black_pieces >> white_reserve >> black_reserve >> next_player;
            std::cin >> init[0] >> init[1] >> init[2] >> init[3] >> init[4] >> init[5] >> next_player;
            if (g != nullptr) {
                g.reset();
            }

            if(false) {
//            if (((init[2]) - 3) != init[4]) {
//                std::cout << INVALID_BOARD_WHITE_PIECES_ERROR << std::endl;
//                //std::cout << init[2] << " " << init[4] << "#" << (init[2] - 3 == 12) << "#" << (init[4] == 12) << std::endl;
//                continue;
//            } else if (init[3] - 3 != init[5]) {
//                std::cout << INVALID_BOARD_BLACK_PIECES_ERROR << std::endl;
//                continue;
            } else {
                std::vector<char> v;
//                int i = 0;
//                while (std::cin.peek() == '\n' || std::cin.peek() == ' ' || std::cin.peek() == EMPTY_PLACE_SYMBOL
//                    || std::cin.peek() == WHITE_SYMBOL || std::cin.peek() == BLACK_SYMBOL) {
//                    //std::cin.ignore(1, '\n');
//                    char c = (char) std::cin.get();
//                    std::cout << "c=" << c << "." << "n=" << (char) std::cin.peek() << "." << std::endl;
//                    if (c == '\n' && std::cin.peek() == '\n')
//                        break;
//
//                    if (c == WHITE_SYMBOL || c == BLACK_SYMBOL || c == EMPTY_PLACE_SYMBOL) {
//                        v.push_back(c);
//                        //std::cout << i << "=" << c << "." << std::endl;
//                        i++;
//                    }
//                }
                int whites = 0;
                int blacks = 0;
                while (!std::cin.eof()) {
                    int n = std::cin.peek();
                    //std::cout << n << std::endl;
                    if (n == '\n' || n == ' ') {
                        std::cin.get();
                        continue;
                    } else if (n == EMPTY_PLACE_SYMBOL || n == WHITE_SYMBOL || n == BLACK_SYMBOL) {
                        //std::cout << (char) n << std::endl;
                        if (n == WHITE_SYMBOL) whites++;
                        else if (n == BLACK_SYMBOL) blacks++;
                        v.push_back((char) std::cin.get());
                    } else
                        break;
                }

                //getline(std::cin, s, '\n');
                //std::cout << "+" << s << "=" << std::endl;
                //std::vector<std::string> v = split_string(ss);
                //std::cout << "x";
                int size = 0;
                int r = init[0];
                for (int j = r; j < ((2 * r) - 1); j++) {
                    size += j;
                }
                size *= 2;
                size += (2*r) - 1;
                //std::cout << "size " << size << " len " << v.size() << std::endl;
                if (v.size() != size) {

                        std::cout << INVALID_BOARD_ROW_ERROR << std::endl;
                    //continue;
                } else {

                    if (((init[2]) - whites) != init[4]) {
                        std::cout << INVALID_BOARD_WHITE_PIECES_ERROR << std::endl;
                        //std::cout << init[2] << " " << init[4] << "#" << (init[2] - 3 == 12) << "#" << (init[4] == 12) << std::endl;
                       // continue;
                    } else if (init[3] - blacks != init[5]) {
                        std::cout << INVALID_BOARD_BLACK_PIECES_ERROR << std::endl;
                        //continue;
                    } else {
                        //std::cout << "OK\n";
                        bool good_state = false;
                        g = std::make_unique<GIPF>(init, next_player, v, good_state);

                        if (good_state)
                            std::cout << BOARD_LOAD_OK << std::endl;
                        else
                            g.reset();
                    }
                }


                //g = std::make_unique<GIPF>(size, killig_number, white_pieces, black_pieces, white_reserve, black_reserve, next_player);

            }
            std::cout << std::endl;
        }
        else if (input == "DO_MOVE") {
            if (g == nullptr) continue;

            g->simple_move();
            std::cout << std::endl;
        }
        else if (input == "PRINT_GAME_BOARD") {
            if (g == nullptr) continue;

            g->print_game_state();
            std::cout << std::endl;
        }


//        if (g != nullptr)
//            g->print_game_state();
    }

    return 0;
}

//Jeden z poniższych stringów:
//
//MOVE_COMMITTED - ruch poprawny, zatwierdzono zmiany na planszy i zmieniono aktualnego gracza.
//BAD_MOVE_<x1>_IS_WRONG_INDEX - zły indeks, pokazuje na pole spoza planszy. W przypadku kiedy podano więcej niż jeden zły indeks, drukowany jest tylko pierwszy napotkany.
//UNKNOWN_MOVE_DIRECTION - nie można określić kierunku ruchu.
//BAD_MOVE_<x1>_IS_WRONG_STARTING_FIELD - wybrano złe, pole startowe (powinno to być jedno z położonych na obrzeżach planszy)
//BAD_MOVE_<x2>_IS_WRONG_DESTINATION_FIELD - wybrano złe, pole docelowe (powinno to być pole wolne albo zawierające pion któregoś z graczy)
//BAD_MOVE_ROW_IS_FULL - nie można wykonać ruchu bo wiersz jest zapełniony.
//W odpowiedzi na PRINT_GAME_BOARD drukowana jest zawartość planszy o identycznym układzie jak ten z instrukcji LOAD_GAME_BOARD.




#include <iostream>
#include "Board.h"
#include "GIPF.h"
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
   Board b(4);
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

    GIPF g(4,4,4,4);
    g.board.read_map();
    g.board.set("3C", 'c');
    g.board.set("2B", 'b');
    g.board.set("5F", 'd');
    g.board.print_cont();
    g.board.print_xN();

    //g.read_move();
    while (true) {
        g.simple_move();
        g.board.print_cont();
    }

    return 0;
}

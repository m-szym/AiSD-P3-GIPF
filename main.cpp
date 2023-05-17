#include <iostream>
#include "Board.h"

int main() {
    //std::cout << "Hello, World!" << std::endl;
    Board b(4);

    b.naive_read();

//    int i = 0;
//    for (auto hkv : b.map) {
//        if (hkv.second == 0)
//            hkv.first.print();
//        //std::cout << hkv.second << " - " << i << "\n\n";
//
//        i++;
//    }
//
//
//    Hex h(0,0);
//    std::cout << "NE " << b[h.hex_neighbour(DIRS::NE)] << std::endl;
//    std::cout << "N " << b[h.hex_neighbour(DIRS::N)] << std::endl;
//    std::cout << "NW " << b[h.hex_neighbour(DIRS::NW)] << std::endl;
//    std::cout << "SE " << b[h.hex_neighbour(DIRS::SE)] << std::endl;
//    std::cout << "SW " << b[h.hex_neighbour(DIRS::SW)] << std::endl;
//    std::cout << "S " << b[h.hex_neighbour(DIRS::S)] << std::endl;

    b.map[b.key_map.at("B5")] = 'b';
    std::cout << "\n\n";
    b.print();
    std::cout << "\n\n";
    b.print_xN();
    std::cout << "\n\n";
    b.print_cont();
    std::cout << "\n\n";
    b.print_rot_l();
    std::cout << "\n\n";

    for (int a = 'A'; a <= 'I'; a++) {
        for (int i = 1; i <= 9; i++) {
            try {
                std::string s = std::string(1, (char) a) + std::to_string(i);
                std::cout << s << " == " << (char) b.map.at(b.key_map.at(s)) << "\n";
            } catch (std::out_of_range& e) {
                std::cout << "out of range\n";
                continue;
            }
        }
        std::cout << std::endl;
    }

    return 0;
}

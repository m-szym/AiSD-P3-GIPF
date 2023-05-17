//
// Created by M Sz on 15/05/2023.
//

#include "Board.h"

Board::Board(int size)
    : real_size(size + 1), playing_field_size(size)
{
    int n = size;
    int a = 0;

    int x = 1;
    char N = 'A';

    for (int q = -n; q <= n; q++) {
        int r1 = std::max(-n, -q - n);
        int r2 = std::min( n, -q + n);
        for (int r = r1; r <= r2; r++) {
            //map.insert(Hex(q, r), 0);
            Hex h(q, r);
            //key_map[std::string(1, N) + std::to_string(x)] = h;
            key_map.emplace(std::string(1, N) + std::to_string(x), h);
            x++;
            std::cout << std::string(1, N) + std::to_string(x) << " == ";
            h.print();
            map[h] = '#';
            a++;
        }
        x = 1;
        N++;
        //map = std::unordered_map<Hex, int>();
        //map[Hex(q, -n)] = 0;
    }
    std::cout << "tiles " << a << std::endl;

//    for (auto kv : map) {
//        for (int i = 0; i < HEX_DIRECTIONS_COUNT; ++i) {
//            try {
//                map.at(kv.first.hex_neighbour(i));
//            } catch (std::out_of_range& e) {
//                //std::cout << "cought\n";
//                kv.second += 1000;
//                map[kv.first] = 0;
//                break;
//            }
//        }
//    }
}

void Board::print() const
{
    int n = playing_field_size;

    int x = 0;
    char N = 'A';

    for (int q = -n; q <= n; q++) {
        int r1 = std::max(-n, -q - n);
        int r2 = std::min( n, -q + n);
        for (int i = -n; i < n - r2 + r1; i++) {
            std::cout << "  ";
        }
        for (int r = r1; r <= r2; r++) {
            //std::cout << "__  ";
            //std::cout << (char) (map.at(Hex(q, r)) + 60) << " ";
            int f = map.at(Hex(q, r));

            std::cout << f << "  ";
            if (f < 10)
                std::cout << " ";

            x++;
        }
        std::cout << std::endl;
        x = 0;
        N++;
    }
}

void Board::print_xN() const
{
    int n = playing_field_size;

    int x = 1;
    char N = 'A';

    for (int q = -n; q <= n; q++) {
        int r1 = std::max(-n, -q - n);
        int r2 = std::min( n, -q + n);
        for (int i = -n; i < n - r2 + r1; i++) {
            std::cout << "  ";
        }
        for (int r = r1; r <= r2; r++) {
            //std::cout << "__  ";
            //std::cout << (char) (map.at(Hex(q, r)) + 60) << " ";
            std::cout << std::string(1, N) + std::to_string(x) << "  ";
            //int f = map.at(Hex(q, r));

//            std::cout << f << "  ";
//            if (f < 10)
//                std::cout << " ";

            x++;
        }
        std::cout << std::endl;
        x = 1;
        N++;
    }
}

void Board::print_cont() const
{
    int n = playing_field_size;

    int x = 0;
    char N = 'A';

    for (int q = -n; q <= n; q++) {
        int r1 = std::max(-n, -q - n);
        int r2 = std::min( n, -q + n);
        for (int i = -n; i < n - r2 + r1; i++) {
            std::cout << " ";
        }
        for (int r = r1; r <= r2; r++) {
            //std::cout << "__  ";
            std::cout << (char) map.at(Hex(q, r)) << " ";
            //std::cout << std::string(1, N) + std::to_string(x) << "  ";
            //int f = map.at(Hex(q, r));

//            std::cout << f << "  ";
//            if (f < 10)
//                std::cout << " ";

            x++;
        }
        std::cout << std::endl;
        x = 0;
        N++;
    }
}

void Board::print_rot_l() const {
    int n = playing_field_size;

    int x = 0;
    char N = 'A';

    for (int q = -n; q <= n; q++) {
        int r1 = std::max(-n, -q - n);
        int r2 = std::min(n, -q + n);
        for (int i = -n; i < n - r2 + r1; i++) {
            std::cout << " ";
        }
        for (int r = r1; r <= r2; r++) {
            //std::cout << "__  ";
            std::cout << (char) map.at(rotate_left(Hex(q, r))) << " ";
            //std::cout << std::string(1, N) + std::to_string(x) << "  ";
            //int f = map.at(Hex(q, r));

//            std::cout << f << "  ";
//            if (f < 10)
//                std::cout << " ";

            x++;
        }
        std::cout << std::endl;
        x = 0;
        N++;
    }
}

int Board::operator[](const Hex& hex) const {
    return map.at(hex);
}

void Board::naive_read() {
    int n = playing_field_size - 1;
    int c = ' ';

    for (int q = -n; q <= n; q++) {
        int r1 = std::max(-n, -q - n);
        int r2 = std::min( n, -q + n);
        for (int i = -n; i < n - r2 + r1; i++) {
            std::cout << " ";
        }
        for (int r = r1; r <= r2; r++) {
            c = getchar();
            while (!(c == EMPTY_PLACE_SYMBOL || c == WHITE_SYMBOL || c == BLACK_SYMBOL)) {
                c = getchar();
            }
            std::cout << (char) c << ".";
            map[rotate_right(Hex(q, r))] = c;
        }
    }

}

//           W _ _ B
//          _ _ _ _ _
//         _ _ _ _ _ _
//        B _ _ _ _ _ W
//         _ _ _ _ _ _
//          _ _ _ _ _
//           W _ _ B




//
// Created by M Sz on 15/05/2023.
//

#include "Board.h"

Board::Board(int size)
    : real_size(size + 1), playing_field_size(size)
{
    int x = GIPF_INDEX_START_INT;
    char N = GIPF_INDEX_START_CHAR;

    for (int q = -size; q <= size; q++) {
        int r1 = std::max(-size, -q - size);
        int r2 = std::min(size, -q + size);
        for (int r = r1; r <= r2; r++) {
            Hex h(q, r);
            key_map.emplace(std::to_string(x) + std::string(1, N), h);
            map.emplace(h, EMPTY_PLACE_SYMBOL);
            x++;
        }
        x = GIPF_INDEX_START_INT;
        N++;
    }

    find_dots();
}

void Board::find_dots() {
    for (auto kv : map) {
        for (int i = 0; i < HEX_DIRECTIONS_COUNT; ++i) {
            try {
                map.at(kv.first.hex_neighbour(i));
            } catch (std::out_of_range& e) {
                //jeśli wchodzimy w to miejsce to ten hex jest krawędziowy (kropka w GIPFie)

                //std::cout << "cought\n";
                //kv.second += 1000;
                map[kv.first] = DOT_SYMBOL;
                break;
            }
        }
    }
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

void Board::print_gipf() const {
    int n = playing_field_size;

    int x = GIPF_INDEX_START_INT;
    char N = GIPF_INDEX_START_CHAR;

    for (int q = -n; q <= n; q++) {
        int r1 = std::max(-n, -q - n);
        int r2 = std::min(n, -q + n);

        for (int i = -n; i < n - r2 + r1; i++) {
            std::cout << SPACER;
        }

        for (int r = r1; r <= r2; r++) {
            std::cout << (char) map.at(rotate_left(Hex(q, r))) << SPACER;
            x++;
        }

        std::cout << std::endl;
        x = GIPF_INDEX_START_INT;
        N++;
    }
}

void Board::print_coords_gipf() const
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

int Board::operator[](const Hex& hex) const {
    try {
        return map.at(hex);
    } catch (std::out_of_range &e) {
        return EMPTY_PLACE_SYMBOL;
    }
}

int Board::operator[](const std::string &s) const {
    try {
        return map.at(key_map.at(s));
    } catch (std::out_of_range &e) {
        return EMPTY_PLACE_SYMBOL;
    }
}

void Board::read_map() {
    int n = playing_field_size - 1;
    int c = ' ';

    for (int q = -n; q <= n; q++) {
        int r1 = std::max(-n, -q - n);
        int r2 = std::min( n, -q + n);
        for (int r = r1; r <= r2; r++) {
            c = getchar();
            while (!(c == EMPTY_PLACE_SYMBOL || c == WHITE_SYMBOL || c == BLACK_SYMBOL)) {
                c = getchar();
            }
            //std::cout << (char) c << ".";
            map[rotate_right(Hex(q, r))] = c;
        }
    }

}

std::vector<Hex> Board::get_fullline(Hex starting_hex, int direction) {
    std::vector<Hex> line;
    Hex current_hex = starting_hex;
    while (map.find(current_hex) != map.end()) {
        if (map.at(current_hex) != DOT_SYMBOL)
            line.push_back(current_hex);
        current_hex = current_hex.hex_neighbour(direction);
    }
    return line;
}

std::vector<Hex> Board::get_wline(Hex starting_hex, int direction) {
    std::vector<Hex> line;
    Hex current_hex = starting_hex;
    while (map.find(current_hex) != map.end()) {
        if (map.at(current_hex) != DOT_SYMBOL)
            if (map.at(current_hex) != EMPTY_PLACE_SYMBOL) {
                line.push_back(current_hex);
            }
            else {
                break;
            }
        current_hex = current_hex.hex_neighbour(direction);
    }
    return line;
}

char Board::set(Hex hex, char value) {
    try {
        char old_value = map.at(hex);
        map[hex] = value;
        return old_value;
    } catch (std::out_of_range& e) {
        std::cout << "Out of range: " << e.what() << std::endl;
        return 0;
    }
}

char Board::set(const std::string& gf1_coords, char value) {
    try {
        char old_value = map.at(key_map.at(gf1_coords));
        map[key_map.at(gf1_coords)] = value;
        return old_value;
    } catch (std::out_of_range& e) {
        std::cout << "Out of range: " << e.what() << std::endl;
        return 0;
    }
}

bool Board::line_has_empty_hex(const std::vector<Hex>& line) {
    for (Hex hex : line) {
        if (map.at(hex) == EMPTY_PLACE_SYMBOL)
            return true;
    }
    return false;
}

void Board::push_line(std::vector<Hex> line, char value) {
//    for (Hex hex : line) {
//        hex.print();
//    }

    //auto it = line.end() - 1;
    auto it = std::find_if(line.begin(), line.end(), [this](Hex hex) { return map.at(hex) == EMPTY_PLACE_SYMBOL; });
    //auto prev = line.end() - 2;
    auto prev = it - 1;
    char tmp = map.at(*prev);

    while (it != line.begin()) {
        if (map.at(*it) == EMPTY_PLACE_SYMBOL) {
            map[*it] = map[*prev];
            map[*prev] = EMPTY_PLACE_SYMBOL;


        }

        //print_cont();

        it--;
        prev--;
    }
    map[line.front()] = value;
}

bool Board::is_dot(Hex hex) const {
    try {
        return map.at(hex) == DOT_SYMBOL;
    } catch (std::out_of_range& e) {
        return false;
    }
}

bool Board::is_dot(const std::string &gf1_coords) const {
    try {
        return map.at(key_map.at(gf1_coords)) == DOT_SYMBOL;
    } catch (std::out_of_range& e) {
        return false;
    }
}












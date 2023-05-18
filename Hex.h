//
// Created by M Sz on 15/05/2023.
//

#ifndef GIPF_HEX_H
#define GIPF_HEX_H

#include <cmath>
#include <vector>
#include <iostream>

class Hex;

const int HEX_DIRECTIONS_COUNT = 6;
enum DIRS {
    SE,
    NE,
    N,
    NW,
    SW,
    S
};

class Hex {
private:

    int iq, ir, is;

public:
    int tag;
    void set_tag(int t) {
        tag = t;
    };
//    Hex(const Hex& other);
//    Hex operator=(const Hex& other) {
//        iq = other.q();
//        ir = other.r();
//        is = other.s();
//        return *this;
//    }
    Hex(int q, int r);
    Hex(int q, int r, int s);   //TODO: throw exeption if s != -q - r

    int distance(const Hex& other) const;
    int length() const;

    Hex hex_direction(int direction) const;
    Hex hex_neighbour(int direction) const;
    int neighbour_direction(const Hex& neighbour) const;

    int q() const;
    int r() const;
    int s() const;

    void print() const {
        std::cout << "q: " << iq << " r: " << ir << " s: " << is << std::endl;
    }

};

bool operator==(const Hex& first, const Hex& second);
bool operator!=(const Hex& first, const Hex& second);
Hex operator+(const Hex& first, const Hex& second);
Hex operator-(const Hex& first, const Hex& second);
Hex operator*(const Hex& first, int k);
Hex operator<=>(const Hex& first, const Hex& second);

int length(const Hex& hex);
int distance(const Hex& first, const Hex& second);

Hex hex_direction(int direction);
Hex hex_neighbour(const Hex& hex, int direction);

Hex rotate_right(const Hex& hex);
Hex rotate_left(const Hex& hex);

const Hex EMPTY_HEX(INT_MAX, INT_MAX);

const std::vector<Hex> hex_directions = {
        {1,  0},
        {1,  -1},
        {0,  -1},
        {-1, 0},
        {-1, 1},
        {0,  1}
};

namespace std {
    template <> struct hash<Hex> {
        size_t operator()(const Hex& h) const {
            hash<int> int_hash;
            size_t hq = int_hash(h.q());
            size_t hr = int_hash(h.r());
            return hq ^ (hr + 0x9e3779b9 + (hq << 6) + (hq >> 2));
        }
    };
}
#endif //GIPF_HEX_H

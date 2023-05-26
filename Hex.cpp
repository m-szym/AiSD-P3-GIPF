//
// Created by M Sz on 15/05/2023.
//

#include "Hex.h"

Hex::Hex(int q, int r)
    : iq(q), ir(r)
{}

int Hex::q() const {
    return iq;
}

int Hex::r() const {
    return ir;
}

int Hex::s() const {
    return -iq -ir;
}


int Hex::distance(const Hex &other) const {
    return (*this - other).length();
}

int Hex::length() const {
    return int((abs(iq) + abs(ir) + abs(s())) / 2);
}

Hex Hex::hex_direction(int direction) {
    return hex_directions[direction];
}

Hex Hex::hex_neighbour(int direction) const {
    return (*this + hex_direction(direction));
}

Hex::Hex(int q, int r, int s) {
    iq = q;
    ir = r;
    //is = s;

}

int Hex::neighbour_direction(const Hex &neighbour) const {
    for (int d = 0; d < HEX_DIRECTIONS_COUNT; d++) {
        if (hex_neighbour(d) == neighbour)
            return d;
    }
    return -1;
}

//Hex::Hex(const Hex& other) {
//    iq = other.q();
//    ir = other.r();
//    is = other.s();
//}


bool operator==(const Hex &first, const Hex &second) {
    if (first.q() == second.q() && first.r() == second.r() && first.s() == second.s())
        return true;
    else
        return false;
}

bool operator!=(const Hex &first, const Hex &second) {
    return !(first == second);
}

Hex operator+(const Hex &first, const Hex &second) {
    return {first.q() + second.q(), first.r() + second.r()};
}

Hex operator-(const Hex &first, const Hex &second) {
    return {first.q() - second.q(), first.r() - second.r()};
}

Hex operator*(const Hex &first, int k) {
    return {first.q() * k, first.r() * k};
}

int length(const Hex &hex) {
    return (int) (abs(hex.q()) + abs(hex.r()) + abs(hex.s())) / 2;
}

int distance(const Hex &first, const Hex &second) {
    return (int) length((first - second));
}

Hex hex_direction(int direction) {
    return hex_directions[direction];
}

Hex hex_neighbour(const Hex &hex, int direction) {
    return (hex + hex_direction(direction));
}

Hex rotate_right(const Hex &hex) {
    return {-hex.r(), -hex.s(), -hex.q()};
}

Hex rotate_left(const Hex &hex) {
    return {-hex.s(), -hex.q(), -hex.r()};
}

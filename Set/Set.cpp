#include "Set.h"
#include <stdexcept>

void Set::insert(int elem) {
    if (elem <= 0 || (size_t)elem > max_power())
        throw std::out_of_range("Set::insert: elem out of range");
    _bitfield.set_bit(elem - 1);
}

void Set::erase(int elem) {
    if (elem <= 0 || (size_t)elem > max_power())
        throw std::out_of_range("Set::erase: elem out of range");
    _bitfield.reset_bit(elem - 1);
}

bool Set::is_member(int elem) const {
    if (elem <= 0 || (size_t)elem > max_power()) return false;
    return const_cast<BitField&>(_bitfield).get_bit(elem - 1);
}

Set& Set::operator+=(const Set& other) {
    if (max_power() != other.max_power())
        throw std::invalid_argument("Set::operator+=: size mismatch");
    _bitfield |= other._bitfield;
    return *this;
}

Set& Set::operator-=(const Set& other) {
    if (max_power() != other.max_power())
        throw std::invalid_argument("Set::operator-=: size mismatch");
    _bitfield &= ~BitField(other._bitfield);
    return *this;
}

Set& Set::operator*=(const Set& other) {
    if (max_power() != other.max_power())
        throw std::invalid_argument("Set::operator*=: size mismatch");
    _bitfield &= other._bitfield;
    return *this;
}

Set Set::operator+(const Set& other) const { Set t = *this; t += other; return t; }
Set Set::operator-(const Set& other) const { Set t = *this; t -= other; return t; }
Set Set::operator*(const Set& other) const { Set t = *this; t *= other; return t; }

Set Set::operator-() const noexcept {
    Set res = *this;
    res._bitfield = ~res._bitfield;
    return res;
}

std::ostream& operator<<(std::ostream& os, const Set& s) {
    os << "{ ";
    bool first = true;
    for (size_t i = 1; i <= s.max_power(); ++i) {
        if (s.is_member((int)i)) {
            if (!first) os << ", ";
            os << i;
            first = false;
        }
    }
    os << " }";
    return os;
}

std::istream& operator>>(std::istream& is, Set& s) {
    char ch;
    if (!(is >> ch) || ch != '{') { is.setstate(std::ios::failbit); return is; }
    Set tmp(s.max_power());
    int elem;
    while (is >> elem) {
        tmp.insert(elem);
        if (!(is >> ch)) break;
        if (ch == '}') break;
        if (ch != ',') { is.setstate(std::ios::failbit); break; }
    }
    s = std::move(tmp);
    return is;
}
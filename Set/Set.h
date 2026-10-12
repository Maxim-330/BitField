#pragma once
#include "BitField.h"

class Set {
    BitField _bitfield;
public:
    Set() : _bitfield() {}
    Set(size_t n) : _bitfield(n) {}
    Set(const Set&) = default;
    Set(Set&&) noexcept = default;
    Set(const BitField& bf) : _bitfield(bf) {}

    operator BitField() const noexcept { return _bitfield; }

    size_t max_power() const noexcept { return _bitfield.size(); }

    void insert(int elem);
    void erase(int elem);
    bool is_member(int elem) const;

    Set& operator+=(int elem) { insert(elem); return *this; }
    Set& operator-=(int elem) { erase(elem);  return *this; }
    Set  operator+ (int elem) const { Set t = *this; t += elem; return t; }
    Set  operator- (int elem) const { Set t = *this; t -= elem; return t; }

    Set& operator+=(const Set& other);
    Set& operator-=(const Set& other);
    Set& operator*=(const Set& other);

    Set operator+(const Set& other) const;
    Set operator-(const Set& other) const;
    Set operator*(const Set& other) const;

    Set operator-() const noexcept;

    Set& operator=(const Set&) = default;
    Set& operator=(Set&&) noexcept = default;

    friend std::ostream& operator<<(std::ostream& os, const Set& s);
    friend std::istream& operator>>(std::istream& is, Set& s);
};
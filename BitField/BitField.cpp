#include "BitField.h"
#include <stdexcept>
#include <string>
#include <algorithm>

BitField::BitField(size_t size) : N(size) {
    _size = (N + 31) / 32;
    _data = new unsigned int[_size]();
}

BitField::BitField(const BitField& other)
    : N(other.N), _size(other._size) {
    _data = _size ? new unsigned int[_size] : nullptr;
    for (size_t i = 0; i < _size; ++i) _data[i] = other._data[i];
}

BitField::BitField(BitField&& other) noexcept
    : _data(other._data), N(other.N), _size(other._size) {
    other._data = nullptr;
    other.N = 0;
    other._size = 0;
}

void BitField::set_bit(size_t pos) {
    if (pos >= N) throw std::out_of_range("BitField::set_bit: pos >= N");
    size_t index = get_index(pos);
    unsigned int mask = get_mask(pos);
    _data[index] |= mask;
}

void BitField::reset_bit(size_t pos) {
    if (pos >= N) throw std::out_of_range("BitField::reset_bit: pos >= N");
    size_t index = get_index(pos);
    unsigned int mask = get_mask(pos);
    _data[index] &= ~mask;
}

bool BitField::get_bit(size_t pos) {
    if (pos >= N) throw std::out_of_range("BitField::get_bit: pos >= N");
    size_t index = get_index(pos);
    unsigned int mask = get_mask(pos);
    return _data[index] & mask;
}

size_t BitField::get_index(size_t pos) const {
    if (pos >= N) throw std::out_of_range("BitField::get_index: pos >= N");
    return pos >> sizeof(unsigned int) * 8;
}

unsigned int BitField::get_mask(size_t pos) const {
    return 1 << (pos % sizeof(unsigned int) * 8);
}

BitField& BitField::operator=(const BitField& other) {
    if (this == &other) return *this;
    unsigned int* new_data = other._size ? new unsigned int[other._size] : nullptr;
    for (size_t i = 0; i < other._size; ++i) new_data[i] = other._data[i];
    delete[] _data;
    _data = new_data;
    N = other.N;
    _size = other._size;
    return *this;
}

BitField& BitField::operator=(BitField&& other) noexcept {
    if (this == &other) return *this;
    delete[] _data;
    _data = other._data;
    N = other.N;
    _size = other._size;
    other._data = nullptr;
    other.N = 0;
    other._size = 0;
    return *this;
}

bool BitField::operator==(const BitField& other) const noexcept {
    if (N != other.N) return false;
    for (size_t i = 0; i < _size; ++i)
        if (_data[i] != other._data[i]) return false;
    return true;
}

BitField BitField::operator~() {
    BitField res(N);
    for (size_t i = 0; i < _size; ++i)
        res._data[i] = ~_data[i];
    return res;
}

BitField BitField::operator|(const BitField& other) {
    if (N != other.N)
        throw std::invalid_argument("BitField::operator|: size mismatch");
    BitField res(N);
    for (size_t i = 0; i < _size; ++i)
        res._data[i] = _data[i] | other._data[i];
    return res;
}

BitField BitField::operator&(const BitField& other) {
    if (N != other.N)
        throw std::invalid_argument("BitField::operator&: size mismatch");
    BitField res(N);
    for (size_t i = 0; i < _size; ++i)
        res._data[i] = _data[i] & other._data[i];
    return res;
}

BitField BitField::operator^(const BitField& other) {
    if (N != other.N)
        throw std::invalid_argument("BitField::operator^: size mismatch");
    BitField res(N);
    for (size_t i = 0; i < _size; ++i)
        res._data[i] = _data[i] ^ other._data[i];
    return res;
}

BitField& BitField::operator|=(const BitField& other) { *this = *this | other; return *this; }
BitField& BitField::operator&=(const BitField& other) { *this = *this & other; return *this; }
BitField& BitField::operator^=(const BitField& other) { *this = *this ^ other; return *this; }

std::ostream& operator<<(std::ostream& os, const BitField& bf) {
    for (size_t i = 0; i < bf.N; ++i)
        os << (bf._data[bf.get_index(i)] & bf.get_mask(i) ? '1' : '0');
    return os;
}

std::istream& operator>>(std::istream& is, BitField& bf) {
    std::string s;
    is >> s;
    if (bf.N == 0) {
        BitField tmp(s.size());
        bf = std::move(tmp);
    }
    size_t n = std::min(s.size(), bf.N);
    for (size_t i = 0; i < n; ++i) {
        if (s[i] == '1') bf.set_bit(i);
        else             bf.reset_bit(i);
    }
    return is;
}
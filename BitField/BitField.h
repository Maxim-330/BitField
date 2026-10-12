#pragma once
#include <cstddef>
#include <iostream>

class BitField {
    unsigned int* _data;
    size_t N;
    size_t _size;

    unsigned int get_mask(size_t pos) const;
    size_t get_index(size_t pos) const;

public:
    BitField() : _data(nullptr), N(0), _size(0) {}
    BitField(size_t size);
    BitField(const BitField& other);
    BitField(BitField&& other) noexcept;
    ~BitField() { delete[] _data; }

    size_t size() const noexcept { return N; }
    size_t words() const noexcept { return _size; }

    void set_bit(size_t pos);
    void reset_bit(size_t pos);
    bool get_bit(size_t pos);

    BitField& operator=(const BitField& other);
    BitField& operator=(BitField&& other) noexcept;

    bool operator==(const BitField& other) const noexcept;
    bool operator!=(const BitField& other) const noexcept { return !(*this == other); }

    BitField operator~();
    BitField operator|(const BitField& other);
    BitField operator&(const BitField& other);
    BitField operator^(const BitField& other);

    BitField& operator|=(const BitField& other);
    BitField& operator&=(const BitField& other);
    BitField& operator^=(const BitField& other);

    friend std::ostream& operator<<(std::ostream& os, const BitField& bf);
    friend std::istream& operator>>(std::istream& is, BitField& bf);
};
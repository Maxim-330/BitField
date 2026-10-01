#include "BitField.h"
#include <iostream>

BitField::BitField(size_t size) : N(size) {
	_size = (N + 31) / 32;
	_data = new unsigned int[_size]();
}

void BitField::set_bit(size_t pos) {
	if (pos >= N) throw std::out_of_range("BitField::get_bit: pos >= N");
	size_t index = get_index(pos);
	unsigned int mask = get_mask(pos);
	_data[index] |= mask;
}

void BitField::reset_bit(size_t pos) {
	if (pos >= N) throw std::out_of_range("BitField::get_bit: pos >= N");
	size_t index = get_index(pos);
	unsigned int mask = get_mask(pos);
	_data[index] &=~ mask;
}

bool BitField::get_bit(size_t pos) {
	if (pos >= N) throw std::out_of_range("BitField::get_bit: pos >= N");
	size_t index = get_index(pos);
	unsigned int mask = get_mask(pos);
	return _data[index] & mask;
}

const size_t BitField::get_index(size_t pos) {
	if (pos >= N) throw std::out_of_range("BitField::get_bit: pos >= N");
	return pos >> sizeof(unsigned int) * 8;
}

const unsigned int BitField::get_mask(size_t pos) {
	return 1 << (pos % sizeof(unsigned int)*8);
}

BitField& BitField::operator=(const BitField other) {
	delete[] _data;

	N = other.N;
	_size = other._size;

	if (other._size > 0) {
		_data = new unsigned int[other._size];
		for (size_t i = 0; i < other._size; ++i)
			_data[i] = other._data[i];
	}
	else {
		_data = nullptr;
	}

	return *this;
}

bool BitField::operator==(const BitField& other) const noexcept {
	if (N != other.N) return false;
	for (size_t i = 0; i < _size; ++i)
		if (_data[i] != other._data[i]) return false;
	return true;
}

BitField BitField::operator|(const BitField& other){
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

BitField BitField::operator~() {
	BitField res(N);
	for (size_t i = 0; i < _size; ++i)
		res._data[i] = ~_data[i];
	return res;
}
#pragma once

class BitField {
	unsigned int* _data;
	size_t N;
	size_t _size;
	const unsigned int get_mask(size_t pos);
	const size_t get_index(size_t size);
public:

	BitField(size_t size);
	// еще конструкторы

	void set_bit(size_t pos);
	void reset_bit(size_t size);
	bool get_bit(size_t pos);

	BitField& operator=(const BitField other);
	bool operator==(const BitField& other) const noexcept;
	bool operator!=(const BitField& other) const noexcept { return !(*this == other); }
	BitField operator~();
	BitField operator|(const BitField& other);
	BitField operator&(const BitField& other);



	~BitField() {};

};
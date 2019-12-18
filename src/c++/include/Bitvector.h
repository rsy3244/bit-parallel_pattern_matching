#pragma once

#include "parameter.h"

#include <cstddef>
#include <vector>

class Bitvector {
	std::vector<Data> m_val;
	std::size_t m_size;

public:
	Bitvector() = default;
	Bitvector(const std::size_t size);
	Bitvector(const Bitvector& obj);
	Bitvector(const std::vector<bool>& obj);

	void set(std::size_t idx, bool flag);
	bool get(std::size_t idx)const;
	std::size_t size()const;

	Bitvector& operator+=(const Bitvector& rhs);
	Bitvector& operator-=(const Bitvector& rhs);
	Bitvector& operator|=(const Bitvector& rhs);
	Bitvector& operator|=(const Data rhs);
	Bitvector& operator&=(const Bitvector& rhs);
	Bitvector& operator^=(const Bitvector& rhs);
	Bitvector& operator<<=(const std::size_t rhs);
	Bitvector& operator>>=(const std::size_t rhs);
	Bitvector operator~() const;
};

Bitvector operator+(const Bitvector& lhs, const Bitvector& rhs);
Bitvector operator-(const Bitvector& lhs, const Bitvector& rhs);
Bitvector operator|(const Bitvector& lhs, const Bitvector& rhs);
Bitvector operator|(const Bitvector& lhs, const Data rhs);
Bitvector operator&(const Bitvector& lhs, const Bitvector& rhs);
Bitvector operator^(const Bitvector& lhs, const Bitvector& rhs);
Bitvector operator<<(const Bitvector& lhs, const std::size_t rhs);
Bitvector operator>>(const Bitvector& lhs, const std::size_t rhs);

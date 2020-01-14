#include "Bitvector.h"
#include "parameter.h"

#include <iostream>
#include <cstddef>
#include <cassert>
#include <bitset>

Bitvector::Bitvector(const std::size_t size): m_capacity((size+WORDSIZE-1)/WORDSIZE), m_val((size+WORDSIZE-1)/WORDSIZE), m_size(size) {}
Bitvector::Bitvector(const Bitvector& obj): m_val(obj.m_val), m_capacity(obj.m_capacity), m_size(obj.m_size) {}
Bitvector::Bitvector(const std::vector<bool> &obj): m_capacity((obj.size()+WORDSIZE-1)/WORDSIZE), m_size(obj.size()) {
	for(std::size_t i = 0; i < m_capacity-1; ++i) {
		Data mask = 0;
		for(std::size_t j = 0; j < WORDSIZE; ++j) {
			mask |= obj[i*WORDSIZE + j] ? (std::size_t(1) << j) : 0;
		}
		m_val.push_back(mask);
	}
	{
		Data mask = 0;
		for(std::size_t j = 0; j < WORDSIZE; ++j) {
			if((m_capacity-1)*WORDSIZE + j == obj.size()) break;
			mask |= obj[(m_capacity-1)*WORDSIZE + j] ? (std::size_t(1) << j) : 0;
		}
		m_val.push_back(mask);
	}
}

void Bitvector::set(std::size_t idx, bool flag) {
	m_val[idx/WORDSIZE] |= (1 << (idx % WORDSIZE));
}

bool Bitvector::get(std::size_t idx) const {
	return m_val[idx/WORDSIZE] & (1 << (idx % WORDSIZE));
}

std::size_t Bitvector::size() const {
	return m_capacity;
}

void Bitvector::dump()const {
	for(auto i = m_capacity; i-- > 0; ) {
		for(auto j = 0; j < WORDSIZE; j+=4){
			std::cerr << std::bitset<4>(m_val[i]>> (WORDSIZE-j-4)) << " ";
		}
	}
	std::cerr << std::endl;
}

Bitvector& Bitvector::operator+=(const Bitvector& rhs) {
	assert(this->size() == rhs.size());
	Data carry = 0;
	for(std::size_t i = 0; i < m_capacity; ++i) {
		m_val[i] += rhs.m_val[i] + carry;
		carry = m_val[i] < rhs.m_val[i];
	}
	return *this;
}

Bitvector& Bitvector::operator-=(const Bitvector& rhs) {
	assert(this->size() == rhs.size());
	Data borrow = 0;
	for(std::size_t i = 0; i < m_capacity; ++i) {
		Data tmp = m_val[i] < rhs.m_val[i];
		m_val[i] -= rhs.m_val[i] + borrow;
		borrow = tmp;
	}
	return *this;
}

Bitvector& Bitvector::operator|=(const Bitvector& rhs) {
	assert(this->size() == rhs.size());
	for(std::size_t i = 0; i < m_capacity; ++i) {
		m_val[i] |= rhs.m_val[i];
	}
	return *this;
}

Bitvector& Bitvector::operator|=(const Data rhs) {
	m_val[0] |= rhs;
	return *this;
}

Bitvector& Bitvector::operator&=(const Bitvector& rhs) {
	assert(this->size() == rhs.size());
	for(std::size_t i = 0; i < m_capacity; ++i) {
		m_val[i] &= rhs.m_val[i];
	}
	return *this;
}

Bitvector& Bitvector::operator^=(const Bitvector& rhs) {
	assert(this->size() == rhs.size());
	for(std::size_t i = 0; i < m_capacity; ++i) {
		m_val[i] ^= rhs.m_val[i];
	}
	return *this;
}

Bitvector Bitvector::operator~() const {
	Bitvector ret(*this);
	for(auto &e : ret.m_val) {
		e = ~e;
	}
	return ret;
}

Bitvector& Bitvector::operator=(Bitvector&& rhs) & noexcept{
	m_val = std::move(rhs.m_val);
	m_size = rhs.m_size;
	m_capacity = rhs.m_capacity;
	return *this;
}

Bitvector& Bitvector::operator<<=(const std::size_t rhs) {
	Data carry = 0;
	for(std::size_t i = 0; i < m_capacity; ++i) {
		m_val[i] = (m_val[i] << rhs)| carry;
		carry = m_val[i] >> (WORDSIZE - rhs);
	}
	return *this;
}

Bitvector& Bitvector::operator>>=(const std::size_t rhs) {
	Data borrow = 0;
	for(std::size_t i = m_capacity; i-- > 0; ) {
		m_val[i] = (m_val[i] >> rhs) | borrow;
		borrow = m_val[i] << (WORDSIZE - rhs);
	}
	return *this;
}


Bitvector operator+(const Bitvector& lhs, const Bitvector& rhs) { return Bitvector(lhs) += rhs; }
Bitvector operator-(const Bitvector& lhs, const Bitvector& rhs) { return Bitvector(lhs) -= rhs; }
Bitvector operator|(const Bitvector& lhs, const Bitvector& rhs) { return Bitvector(lhs) |= rhs; }
Bitvector operator|(const Bitvector& lhs, const Data rhs) { return Bitvector(lhs) |= rhs; }
Bitvector operator&(const Bitvector& lhs, const Bitvector& rhs) { return Bitvector(lhs) &= rhs; }
Bitvector operator^(const Bitvector& lhs, const Bitvector& rhs) { return Bitvector(lhs) ^= rhs; }
Bitvector operator<<(const Bitvector& lhs, const std::size_t rhs) { return Bitvector(lhs) <<= rhs; }
Bitvector operator>>(const Bitvector& lhs, const std::size_t rhs) { return Bitvector(lhs) >>= rhs; }

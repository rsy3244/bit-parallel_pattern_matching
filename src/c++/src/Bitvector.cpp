#include "Bitvector.h"
#include "parameter.h"

#include <cstddef>

Bitvector::Bitvector(const std::size_t size): m_val((size+WORDSIZE-1)/WORDSIZE), m_size(size) {}
Bitvector::Bitvector(const Bitvector& obj): m_val(obj.m_val), m_size(obj.m_size) {}
Bitvector::Bitvector(const std::vector<bool> &obj): m_size((obj.size()+WORDSIZE-1)/WORDSIZE) {
	for(std::size_t i = 0; i < m_size-1; ++i) {
		Data mask = 0;
		for(std::size_t j = 0; j < WORDSIZE; ++j) {
			mask |= obj[i*WORDSIZE + j] ? (std::size_t(1) << j) : 0;
		}
		m_val.push_back(mask);
	}
	{
		Data mask = 0;
		for(std::size_t j = 0; j < WORDSIZE; ++j) {
			if((m_size-1)*WORDSIZE + j == obj.size()) break;
			mask |= obj[(m_size-1)*WORDSIZE + j] ? (std::size_t(1) << j) : 0;
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
	return m_size;
}

Bitvector& Bitvector::operator+=(const Bitvector& rhs) {
	Data carry = 0;
	for(std::size_t i = 0; i < m_size; ++i) {
		m_val[i] += rhs.m_val[i] + carry;
		carry = m_val[i] < rhs.m_val[i];
	}
	return *this;
}

Bitvector& Bitvector::operator-=(const Bitvector& rhs) {
	Data borrow = 0;
	for(std::size_t i = 0; i < m_size; ++i) {
		Data tmp = m_val[i] < rhs.m_val[i];
		m_val[i] -= rhs.m_val[i] + borrow;
		borrow = tmp;
	}
	return *this;
}
	
Bitvector& Bitvector::operator|=(const Bitvector& rhs) {
	for(std::size_t i = 0; i < m_size; ++i) {
		m_val[i] |= rhs.m_val[i];
	}
	return *this;
}

Bitvector& Bitvector::operator|=(const Data rhs) {
	m_val[0] |= rhs;
	return *this;
}
	
Bitvector& Bitvector::operator&=(const Bitvector& rhs) {
	for(std::size_t i = 0; i < m_size; ++i) {
		m_val[i] &= rhs.m_val[i];
	}
	return *this;
}
	
Bitvector& Bitvector::operator^=(const Bitvector& rhs) {
	for(std::size_t i = 0; i < m_size; ++i) {
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

Bitvector& Bitvector::operator<<=(const std::size_t rhs) {
	Data carry = ((Data(1) << (WORDSIZE-1)) & m_val[0]) >> (WORDSIZE-1);
	for(std::size_t i = 1; i < m_size; ++i) {
		m_val[i] = (m_val[i] << std::size_t(1)) | carry;
		carry = ((Data(1) << (WORDSIZE-1)) & m_val[i]) >> (WORDSIZE-1);
	}
	return *this;
}

Bitvector& Bitvector::operator>>=(const std::size_t rhs) {
	Data borrow = (m_val[m_size-1] & 1) << (WORDSIZE -1);
	for(std::size_t i = m_size-1; i-- > 0; ) {
		m_val[i] = (m_val[i] >> std::size_t(1)) | borrow;
		borrow = (m_val[i-1] & 1) << (WORDSIZE -1);
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

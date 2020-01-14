#include "TNFA.h"
#include "Parser.h"
#include "parameter.h"
#include "Bitmask.h"

#include <iostream>
#include <bitset>
#include <cassert>
	
TNFA::TNFA(const std::string &pattern_): pattern(pattern_), length(pattern_.size()){
	Parser parser(pattern);
	depth = parser.mdepth;
	state_size = parser.BLK[0].size();

	for(std::size_t i = 0; i < depth; ++i) {
		m_scatter.push_back(Bitmask(parser.BLK[0][i], parser.SRC[0][i], parser.DST[0][i]));
		m_gather.push_back(Bitmask(parser.BLK[1][i], parser.SRC[1][i], parser.DST[1][i]));
		m_propagate.push_back(Bitmask(parser.BLK[2][i], parser.SRC[2][i], parser.DST[2][i]));
	}
	for(std::size_t c = 0; c < SIGMA; ++c) { 
		CHR[c] = Bitvector(parser.CHR[c]);
	}
	for(std::size_t c = 0; c < SIGMA; ++c) { 
		REP[c] = Bitvector(parser.REP[c]);
	}
}

void TNFA::scatter(const std::size_t k) {
	state |= ((m_scatter[k].BLK - (state & m_scatter[k].SRC)) & m_scatter[k].DST);
}

void TNFA::gather(const std::size_t k) {
	state |= ((m_gather[k].BLK + (state & m_gather[k].SRC)) & m_gather[k].DST);
}

void TNFA::propagate(const std::size_t k) {
	Bitvector a(m_propagate[k].DST);
	a |= (state & m_propagate[k].BLK);
	state |= (m_propagate[k].BLK & ((~(a - m_propagate[k].SRC)) ^ a));
}

void TNFA::move(const char c) {
	state = (((state << std::size_t(1)) & CHR[c]) | 1) | (state & REP[c]);
}

void TNFA::eps_closure() {
	for(std::size_t k = depth; k > 0; --k ) {
		propagate(k);
		gather(k-1);
	}
	propagate(0);
	for(std::size_t k = 1; k < depth+1; ++k) {
		scatter(k-1);
		propagate(k);
	}
}

bool TNFA::match(const std::string &text) {
	state = Bitvector(state_size);
	state.set(0, true);
	eps_closure();
	for(auto e : text) {
		if(state.get(accept)) return true;
		move(e);
		eps_closure();
	}
	if(state.get(accept)) return true;
	return false;
}

		

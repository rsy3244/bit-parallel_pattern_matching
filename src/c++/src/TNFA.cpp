#include "TNFA.h"
#include "Parser.h"
	
TNFA::TNFA(const std::string &pattern_): pattern(pattern_), length(pattern_.size()){
	Parser parser(pattern);
	for(std::size_t i = 0; i < 3; ++i) {
		for(std::size_t j = 0; j <  parser.BLK[i].size(); ++j) {
			BLK[i].push_back(Bitvector(parser.BLK[i][j]));
			SRC[i].push_back(Bitvector(parser.SRC[i][j]));
			DST[i].push_back(Bitvector(parser.DST[i][j]));
		}
	}
	for(std::size_t c = 0; c < 128; ++c) { 
		CHR[c] = Bitvector(parser.CHR[c]);
	}
	for(std::size_t c = 0; c < 128; ++c) { 
		REP[c] = Bitvector(parser.REP[c]);
	}
	vec_size = BLK[0][0].size();
	accept = parser.BLK[0][0].size();
}

void TNFA::scatter(const std::size_t k) {
	state |= ((BLK[0][k] - (state & SRC[0][k])) & DST[0][k]);
}

void TNFA::gather(const std::size_t k) {
	state |= ((BLK[1][k] + (state & SRC[1][k])) & DST[1][k]);
}

void TNFA::propagate(const std::size_t k) {
	Bitvector a = (state & BLK[2][k]) | DST[2][k];
	state |= (BLK[2][k] & ((~(a - SRC[2][k])) ^ a));
}

void TNFA::move(const char c) {
	state = (((state << std::size_t(1)) & CHR[c] | 1) | (state & REP[c]));
}

void TNFA::eps_closure() {
	for(std::size_t k = depth; k > 0; --k ) {
		propagate(k);
		gather(k-1);
	}
	propagate(0);
	for(std::size_t k = 0; k < depth+1; ++k) {
		scatter(k-1);
		propagate(k);
	}
}

bool TNFA::match(const std::string &text) {
	state = Bitvector(vec_size);
	state.set(0, true);
	eps_closure();
	for(auto e : text) {
		if(state.get(accept)) return true;
		move(e);
		eps_closure();
	}
	return false;
}

		

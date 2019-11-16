#pragma once
#include "parameter.h"

class TNFA {
	using Data = size_t;
	constexpr WORDSIZE = sizeof(Data);
	const std::string pattern;
	std::unordered_map<std::vector<Data>> CHR;
	std::unordered_map<std::vector<Data>> REP;

	std::vector<Data> BLK[3];
	std::vector<Data> SRC[3];
	std::vector<Data> DST[3];

	std::vector<Data> state;

	void scatter(const size_t depth);
};		







	

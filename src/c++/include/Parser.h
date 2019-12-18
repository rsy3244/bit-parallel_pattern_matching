#pragma once

#include "parameter.h"
#include "TNFA.h"

#include <cstddef>
#include <string>
#include <vector>


class Parser {
	friend TNFA;
	const std::string pattern;
	const std::size_t length;
	std::size_t mdepth;

	std::vector<bool> CHR[SIGMA];
	std::vector<bool> REP[SIGMA];

	std::vector<std::vector<bool>> BLK[3];
	std::vector<std::vector<bool>> SRC[3];
	std::vector<std::vector<bool>> DST[3];

	void add_node();
	void add_depth();
	std::size_t parse_int(std::size_t &idx);

	void expr(std::size_t &idx, std::size_t depth);
	void factor(std::size_t &idx, std::size_t depth);
	void primary(std::size_t &idx, std::size_t depth);
	void atom(std::size_t &idx, std::size_t depth);
	

public:
	Parser(const std::string pattern_);
};

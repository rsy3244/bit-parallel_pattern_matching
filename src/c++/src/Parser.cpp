#include "Parser.h"

#include <cstddef>
#include <cassert>

void Parser::add_node() {
	for(std::size_t i = 0; i < 3; ++i) {
		for(auto &e : BLK[i]) e.push_back(false);
		for(auto &e: SRC[i]) e.push_back(false);
		for(auto &e: DST[i]) e.push_back(false);
	}
	for(auto &e : CHR) {
		e.push_back(false);
	}
	for(auto &e : REP) {
		e.push_back(false);
	}
}


std::size_t Parser::parse_int(std::size_t &idx) {
	std::size_t ret = pattern[idx] - '0';
	while(isdigit(pattern[++idx])) {
		ret = ret*10 + pattern[idx] - '0';
	}
	return ret;
}


/* 
 * BNF 
 * expr := factor | factor expr
 * factor := primary | primary '|' factor
 * primary := atom | '(' expr ')' | '(' expr ')*'
 * atom :=  alpha | alpha'?' | '.' | alpha'*' | alpha'+' | alpha'{'num','num'}'
 */

void Parser::expr(std::size_t &idx, std::size_t depth) {
	while(idx < length && pattern[idx] != ')') {
		factor(idx, depth);
	}
}

void Parser::factor(std::size_t &idx, std::size_t depth) {
	std::size_t src = BLK[0][depth].size()-1;
	SRC[0][depth][src] = true;
	add_node();
	DST[0][depth].back() = true;
	primary(idx, depth+1);
	SRC[1][depth].back() = true;
	while(pattern[idx] == '|') {
		add_node();
		DST[0][depth].back() = true;
		primary(idx, depth+1);
		SRC[1][depth].back() = true;
	}
	add_node();
	BLK[0][depth].back() = true;
	DST[1][depth].back() = true;
	std::size_t dst = BLK[0][depth].size()-1;
	for(std::size_t i = src+1; i < dst; ++i) {
		BLK[1][depth][i] = true;
	}
}

void Parser::primary(std::size_t &idx, std::size_t depth) {
	if(pattern[idx] == '('){
		++idx;
		expr(idx, depth);
		assert(pattern[idx] == ')');
		++idx;
	} else {
		atom(idx, depth);
	}
}

void Parser::atom(std::size_t &idx, std::size_t depth) {
	add_node();
	if(pattern[idx] == '.') {
		for(auto &e : CHR) e.back() = true;
		++idx;
		
	} else {
		assert(isalpha(pattern[idx]));
		CHR[pattern[idx]].back() = true;
		++idx;
		switch(pattern[idx]) {
			case '?':
				BLK[2][depth].back() = true;
				break;
			case '*': BLK[2][depth].back() = true;
				REP[pattern[idx]].back() = true;
				break;
			case '+': 
				REP[pattern[idx]].back() = true;
				break;
			case '{': {
				std::size_t x = parse_int(idx);
				assert(pattern[idx] == ',');
				std::size_t y = parse_int(idx);
				assert(pattern[idx] == '}');
				assert(y > x);
				for(std::size_t i = 0; i < y-x; i++) {
					CHR[pattern[idx]].back() = true;
					BLK[2][depth].back() = true;
					add_node();
				}
				for(std::size_t i = 0; i < x; i++) {
					CHR[pattern[idx]].back() = true;
					add_node();
				}
			} break;
		}
		++idx;
	}
}


Parser::Parser(const std::string pattern_): pattern(pattern_), length(pattern.size()){
	add_node();
	size_t idx = 0;
	expr(idx, 0);
}

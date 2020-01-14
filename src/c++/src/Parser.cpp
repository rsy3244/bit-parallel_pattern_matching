#include "Parser.h"

#include <cstddef>
#include <cassert>
#include <iostream>
#include <vector>

void Parser::add_depth() {
	std::size_t vec_size = BLK[0].size() == 0 ? 0 : BLK[0].front().size();
	for(std::size_t i = 0; i < 3; ++i) {
		BLK[i].push_back(std::vector<bool>(vec_size));
		SRC[i].push_back(std::vector<bool>(vec_size));
		DST[i].push_back(std::vector<bool>(vec_size));
	}
}

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
 * expr := factor | factor '|' expr
 * factor := primary | primary factor
 * primary := atom | '(' expr ')' | '(' expr ')*'
 * atom :=  alpha | alpha'?' | '.' | alpha'*' | alpha'+' | alpha'{'num','num'}'
 */

void Parser::expr(std::size_t &idx, std::size_t depth) {
	auto check_union = [&](){
		std::size_t d = 1;
		for(std::size_t i = idx; d > 0 && i < pattern.size(); ++i) {
			if(pattern[i] == '(') ++d;
			if(pattern[i] == ')') --d;
			if(pattern[i] == '|' && d == 1) return true;
		}
		return false;
	};
	if(!check_union()){
		factor(idx, depth);
		return;
	}
	if(depth+1 > mdepth){
		mdepth += 1;
		add_depth();
	}
	std::size_t src = BLK[0][depth].size()-1;
	SRC[0][depth][src] = true;
	add_node();
	DST[0][depth].back() = true;
	factor(idx, depth+1);
	SRC[1][depth].back() = true;
	while(pattern[idx] == '|') {
		add_node();
		DST[0][depth].back() = true;
		factor(++idx, depth+1);
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

void Parser::factor(std::size_t &idx, std::size_t depth) {
	while(idx < length && pattern[idx] != ')' && pattern[idx] != '|') {
		primary(idx, depth);
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
			default:
				--idx;
		}
		++idx;
	} else {
		assert(isalpha(pattern[idx]));
		auto back = CHR[pattern[idx]].size()-1;
		CHR[pattern[idx]][back] = true;
		++idx;
		switch(pattern[idx]) {
			case '?':
				BLK[2][depth][back] = true;
				BLK[2][depth][back-1] = true;
				break;
			case '*': 
				BLK[2][depth].back() = true;
				BLK[2][depth][back-1] = true;
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
					BLK[2][depth][back-1] = true;
					add_node();
				}
				for(std::size_t i = 0; i < x; i++) {
					CHR[pattern[idx]].back() = true;
					add_node();
				}
			} break;
			default:
				--idx;
		}
		++idx;
	}
}


Parser::Parser(const std::string pattern_):
	pattern(pattern_), length(pattern.size()), mdepth(0)
{
	add_depth();
	add_node();
	size_t idx = 0;
	expr(idx, 0);
	mdepth += 1;
	std::size_t size = BLK[0][0].size();
	std::size_t src = 0;
	for(std::size_t d = mdepth-1; d > 0; --d) {
		for(std::size_t i = 0; i < size-1; ++i) {
			if(SRC[0][d-1][i]) src = i;
			if(DST[0][d-1][i]) {
				auto j = i;
				for(; !SRC[1][d-1][j]; ++j) {
					if(!BLK[2][d][j]){
						i = j;
						break;
					}
				}
				if(j != i) {
					for(; !DST[1][d-1][i]; ++i) {
					}
					BLK[2][d-1][src] = true;
					BLK[2][d-1][i] = true;
					src = i;
				}
			}
		}
	}
	for(std::size_t d = 0; d < mdepth; ++d) {
		for(std::size_t i = 0; i < size-1; ++i) {
			if(BLK[2][d][i] && !BLK[2][d][i+1]) {
				DST[2][d][i] = true;
			}
			if(!BLK[2][d][i] && BLK[2][d][i+1]) {
				SRC[2][d][i] = true;
			}
		}
	}
}

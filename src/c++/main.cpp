#include "include/TNFA.h"
#include "Bitvector.h"
#include <cstddef>
#include <iostream>
#include <string>

signed main(){
	std::string pattern;
	std::cerr << "input pattern" << std::endl;
	std::cin >> pattern;
	TNFA tnfa(pattern);
	std::cerr << "input number of text" << std::endl;
	std::size_t n;
	std::cin >> n;
	std::cin.ignore(1,'\n');
	std::cerr << "input text" << std::endl;
	for(std::size_t i = 0; i < n; ++i) {
		std::string text;
		getline(std::cin, text);
		std::cout << tnfa.match(text) << std::endl;
	}

	std::vector<bool> b = {1,0,0,1};
	Bitvector a =  Bitvector(b) << 1;
	a.dump();
	std::cerr << std::endl;

	return 0;
}


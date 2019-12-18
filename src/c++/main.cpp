#include "include/TNFA.h"
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
	std::cerr << "input text" << std::endl;
	for(std::size_t i = 0; i < n; ++i) {
		std::string text;
		getline(std::cin, text);
		std::cout << tnfa.match(text) << std::endl;
	}

	return 0;
}


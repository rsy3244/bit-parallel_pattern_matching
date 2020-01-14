CXX := g++
CXXFLAGS := -Wall -g -std=c++17 -fsanitize=undefined -fvisibility=hidden -I include/

main: main.o Bitvector.o Bitmask.o TNFA.o Parser.o
	$(CXX) main.o Bitvector.o Bitmask.o TNFA.o Parser.o $(CXXFLAGS) -o $@ $(LDFLAGS)

main.o: src/main.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

TNFA.o: src/TNFA.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

Parser.o: src/Parser.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

Bitvector.o: src/Bitvector.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

Bitmask.o: src/Bitmask.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@


.PHONY: clean

clean:
	$(RM) -r *.o



PROGRAM = main
OBJS = main.o
CXX = g++
CXXFLAGS = -std=c++17 -Wall -fsanitize=undefined -fvisibility=hidden
.SUFFIXES: .cpp .o

.PHONY: all

all: depend $(PROGRAM)

$(PROGRAM): $(OBJS)
	$(CXX) -o $(PROGRAM) $^

.c.o:
	$(CXX) $(CXXFALGS) -c $<

.PHONY: clean

clean:
	$(RM) $(PROGRAM) $(OBJS) depend.inc

.PHONY: depend

depend: $(OBJS:.o=.cpp)
	-@ $(RM) depend.inc
	-@ for i in $^; do $(CXX) -MM $$i | sed "s/\ [_a-zA-Z0-9] [_a-zA-Z0-9]*\.cpp//g" >> depend.inc

-include depend.inc

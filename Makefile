CXX ?= g++
CXXFLAGS ?= -Wall -Werror -pedantic -g --std=c++11

sandbox.exe: BigInt.cpp sandbox.cpp
	$(CXX) $(CXXFLAGS) $^ -o $@

.PHONY: clean
clean:
	rm -rvf *.exe *.o *.dSYM *.gch *.stackdump *.out
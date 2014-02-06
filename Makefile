CXX = g++
CPPFLAGS = -std=c++11 -Wall -Wextra -Werror -pedantic-errors
CPPDEBUG = -Og -g
CPPOPTIM = -O3
LDLIB = -pthread

matrix: main.o
	$(CXX) $(CPPFLAGS) $(CPPDEBUG) $^ -o $@ $(LDLIB)

main.o: main.cpp matrix.h exceptions.h
	$(CXX) $(CPPFLAGS) $(CPPDEBUG) -c $< $(LDLIB)

clean:
	rm -rf *.o matrix

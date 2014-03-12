CXX = clang++
CPPFLAGS = -std=c++11 -Wall -Wextra -Werror -pedantic-errors
CPPDEBUG = -Og -g
CPPOPTIM = -O3
LDLIB = -pthread

matrix: main.o
	$(CXX) $(CPPFLAGS) $(CPPOPTIM) $^ -o $@ $(LDLIB)

main.o: main.cpp matrix.h exceptions.h tQueue.h
	$(CXX) $(CPPFLAGS) $(CPPOPTIM) -c $< $(LDLIB)

clean:
	rm -rf *.o matrix

CXX = g++
CPPFLAGS = -std=c++11 -Wall -Wextra -Werror -pedantic-errors
CPPDEBUG = -Og -g -fsanitize=undefined
CPPOPTIM = -O3
#LDFLAGS

matrix: main.o
	$(CXX) $(CPPFLAGS) $(CPPOPTIM) $^ -o $@

main.o: main.cpp matrix.h exceptions.h
	$(CXX) $(CPPFLAGS) $(CPPOPTIM) -c $<

clean:
	rm -rf *.o matrix

CXX = g++
CPPFLAGS = -std=c++11 -Wall -Wextra -pedantic
CPPDEBUG = -Og -g
CPPOPTIM = -O3
LDLIB = -pthread

matrix: main.o
	$(CXX) $(CPPFLAGS) $(CPPDEBUG) $^ -o $@ $(LDLIB)

main.o: main.cpp matrix.h exceptions.h tQueue.h
	$(CXX) $(CPPFLAGS) $(CPPDEBUG) -c $< $(LDLIB)

clean:
	rm -rf *.o matrix

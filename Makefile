CXX = g++
CPPFLAGS = -std=c++11 -Wall -Wextra -pedantic
CPPDEBUG = -Og -g
CPPOPTIM = -O3
LDLIB = -pthread

trainz: main.o track.o
	$(CXX) $(CPPFLAGS) $(CPPOPTIM) $^ -o $@ $(LDLIB)

main.o: main.cpp track.h
	$(CXX) $(CPPFLAGS) $(CPPOPTIM) -c $< $(LDLIB)

track.o: track.cpp track.h
	$(CXX) $(CPPFLAGS) $(CPPOPTIM) -c $< $(LDLIB)

clean:
	rm -rf *.o trainz
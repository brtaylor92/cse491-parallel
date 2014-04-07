CXX = g++
CPPFLAGS = -std=c++11 -Wall -Wextra -pedantic
CPPDEBUG = -Og -g
CPPOPTIM = -O3

trainz: main.o track.o
	$(CXX) $(CPPFLAGS) $(CPPDEBUG) $^ -o $@ 

main.o: main.cpp track.h intersection.h
	$(CXX) $(CPPFLAGS) $(CPPDEBUG) -c $< 

track.o: track.cpp track.h
	$(CXX) $(CPPFLAGS) $(CPPDEBUG) -c $<

clean:
	rm -rf *.o trainz

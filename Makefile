CXX = clang++
CPPFLAGS = -std=c++11 -Wall -Wextra -pedantic
CPPDEBUG = -Og -g
CPPOPTIM = -O2

trainz: main.o track.o
	$(CXX) $(CPPFLAGS) $(CPPOPTIM) $^ -o $@ 

main.o: main.cpp track.h
	$(CXX) $(CPPFLAGS) $(CPPOPTIM) -c $< 

track.o: track.cpp track.h
	$(CXX) $(CPPFLAGS) $(CPPOPTIM) -c $<

clean:
	rm -rf *.o trainz

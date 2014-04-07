#include <array>
#include <algorithm>
#include <iostream>
#include <random>
#include <string>

#include "track.h"

using std::array;
using std::default_random_engine;
using std::uniform_int_distribution;
using std::cerr;
using std::cout;
using std::endl;
using std::stoi;

void rand(array<Track, 5> &world, uint32_t trackLength, uint32_t numTrains, uint32_t trainSpeed) {
  default_random_engine dre;
  uniform_int_distribution<uint32_t> trackGen(1, 4), locGen(0, trackLength-1);
  
  uint32_t track;
  for(uint32_t i = 0; i<numTrains; i++) {
    do{ track = trackGen(dre); } while(world[track].full());
    while(!world[track].letThereBeTrain(locGen(dre), trainSpeed, trainSpeed));
  }
}

int main(int argc, char const *argv[])
{
	if(argc != 5) {
		cerr << "Format: " << argv[0] << " [trackLength] [numTrains] [trainSpeed] [numSteps]"  << endl;
    return 1;
  }

  if(stoi(argv[2]) > 4*stoi(argv[1])) {
    cerr << "SHE CANNAE TAKE ANY MORE TRAINS CAP'N" << endl;
    return 1;
  }

  /* code */
  array<Track, 5> world = {{stoi(argv[1]), stoi(argv[1]), stoi(argv[1]), stoi(argv[1]), stoi(argv[1])}}; 
  
  rand(world, stoi(argv[1]), stoi(argv[2]), stoi(argv[3]));

  for(uint32_t i = 0; i < world.size(); i++) {
    cout << "track " << i << " (length: " << world[i].capacity() << ")" << endl << world[i] << endl << endl;
  }

  (void) world;

	return 0;
}

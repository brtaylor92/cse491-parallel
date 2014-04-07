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
using std::stoul;

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

  uint64_t trackLen(stoul(argv[1])), numTrains(stoul(argv[2])), 
           speed(stoul(argv[3])), numSteps(stoul(argv[4]));

  if(numTrains > 4*trackLen) {
    cerr << "SHE CANNAE TAKE ANY MORE TRAINS CAP'N" << endl;
    return 1;
  }

  if(speed > trackLen) {
    cerr << "This isn't FTLT: Faster Than Light Trains" << endl;
    return 1;
  }

  //center, l, r, t, b
  array<Track, 5> world{{Track(trackLen, 1, 2), Track(trackLen, 2, 0), Track(trackLen, 0, 1),
                        Track(trackLen, 4, 4), Track(trackLen, 3, 3)}}; 
  
  rand(world, trackLen, numTrains, speed);

  for(uint32_t i = 0; i < world.size(); i++) {
    cout << "track " << i << " (length: " << world[i].capacity() << ")" << endl << world[i] << endl << endl;
  }

  for(uint64_t i = 0; i<numSteps; i++) {
    for(auto &t: world) {
      t.refresh();
      t.babystep();
    }
    
    /* comm */
    
    for(auto &t: world)
      t.babystep();
  }
  for(uint32_t i = 0; i < world.size(); i++) {
    cout << "track " << i << " (length: " << world[i].capacity() << ")" << endl << world[i] << endl << endl;
  }


	return 0;

}

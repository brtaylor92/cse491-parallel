#include <array>
#include <algorithm>
#include <iostream>
#include <random>
#include <string>

#include "track.h"
#include "intersection.h"

using std::array;
using std::default_random_engine;
using std::uniform_int_distribution;
using std::cerr;
using std::cout;
using std::endl;
using std::stoi;
using std::stoul;

void rand(array<Track*, 5> &world, uint32_t trackLength, uint32_t numTrains, uint32_t trainSpeed) {
  default_random_engine dre;
  uniform_int_distribution<uint32_t> trackGen(1, 4), locGen(0, trackLength-1);
  
  uint32_t track;
  for(uint32_t i = 0; i<numTrains; i++) {
    do{ track = trackGen(dre); } while((*world[track]).full());
    while(!(*world[track]).letThereBeTrain(locGen(dre), trainSpeed, trainSpeed));
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
    cerr << "SHE CANNAE TAKE ANY MORE TRAINS CAP'N (numTrains < 4*trackLen)" << endl;
    return 1;
  }

  if(speed > trackLen) {
    cerr << "This isn't FTLT: Faster Than Light Trains (trainSpeed < trackLen)" << endl;
    return 1;
  }

  //center, l, r, t, b
  Intersection grid0(trackLen, 1, 2, 3, 4);
  Track grid1(trackLen, 2, 0);
  Track grid2(trackLen, 0, 1);
  Track grid3(trackLen, 4, 0);
  Track grid4(trackLen, 0, 3);
  array<Track*, 5> world{{&grid0, &grid1, &grid2, &grid3, &grid4}}; 
  
  rand(world, trackLen, numTrains, speed);

  for(uint64_t i = 0; i<numSteps; i++) {
    cout << "timestep " << i <<":" << endl;
    for(uint32_t i = 0; i < world.size(); i++) {
      cout << "track " << i << " (length: " << ((*world[i]).capacity()) << ")" << endl << (*world[i]) << endl << endl;
    }
    if(i%3 == 2) {
      (*world[0]).turn();
      cout << endl << "TURN" << endl << endl;
    }
    
    for(auto t: world) {
      (*t).refresh();
      (*t).babystep();
    } 
     
    for(auto a: vector<int>{0, 1, 2, 3, 4, 0}) {
      Track *curr = world[a], *next = world[(*curr).getNext()];
      if(curr == world[(*next).getPrev()]) {
        auto cats = (*next).freeSlots();
        auto dogs = (*curr).sendTrains(cats);
        (*next).addTrains(dogs);
        (*curr).babystep();
      }
    }
    cout << endl;
  }

  cout << "timestep " << numSteps <<":" << endl;
  for(uint32_t i = 0; i < world.size(); i++) {
    cout << "track " << i << " (length: " << (*world[i]).capacity() << ")" << endl << *(world[i]) << endl << endl;
  }

  
  

	return 0;

}

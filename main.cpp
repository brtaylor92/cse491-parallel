#include <array>
#include <algorithm>
#include <iostream>
#include <random>
#include <string>
#include <mpi.h>

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

void rand(Track* t, uint32_t trackLength, uint32_t numTrains, 
          uint32_t trainSpeed, int rank) {
  default_random_engine dre(rank);
  uniform_int_distribution<uint32_t> trackGen(1, 4), locGen(0, trackLength-1);
  
  for(uint32_t i = 0; i<numTrains; i++) {
    while(!t->letThereBeTrain(locGen(dre), trainSpeed, trainSpeed));
  }
}

int main(int argc, char const *argv[])
{
	if(argc != 5) {
		cerr << "Format: " << argv[0] 
      << " [trackLength] [starting trains per segment] [trainSpeed] [numSteps]"  << endl;
    return 1;
  }

  uint64_t trackLen(stoul(argv[1])), numTrains(stoul(argv[2])), 
           speed(stoul(argv[3])), numSteps(stoul(argv[4]));

  if(numTrains > 4*trackLen) {
    cerr 
      << "SHE CANNAE TAKE ANY MORE TRAINS CAP'N (numTrains < 4*trackLen)" 
      << endl;
    return 1;
  }

  if(speed > trackLen) {
    cerr 
      << "This isn't FTLT: Faster Than Light Trains (trainSpeed < trackLen)" 
      << endl;
    return 1;
  }

  MPI_Init(&argc, &argv);
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  Track* myTrack;

  //center, l, r, t, b
  switch(rank) {
    case 0:
      myTrack = new Intersection grid0(trackLen, 1, 2, 3, 4);
      break;
    case 1:
      myTrack = new Track grid1(trackLen, 2, 0);
      break;
    case 2:
      myTrack = new Track grid2(trackLen, 0, 1);
      break;
    case 3:
      myTrack = new Track grid3(trackLen, 4, 0);
      break;
    case 4:
      myTrack = new Track grid4(trackLen, 0, 3);
      break;
  }
  
  rand(myTrack, trackLen, numTrains, speed, rank);

  for(uint64_t i = 0; i<numSteps; i++) {
    if(rank == 0) cout << "timestep " << i <<":" << endl;
    
    for(uint32_t i = 0; i < world.size(); i++) {
      cout << "track " << rank << " (length: " << (myTrack->capacity()) 
        << ")" << endl << *myTrack << endl << endl;
    }
    if(rank == 0 && i%3 == 2) {
      myRank->turn();
      cout << endl << "TURN" << endl << endl;
    }
    
    myRank->refresh();
    myRank->babystep();
    myTrack->communicate;
    myTrack->babystep();
    myTrack->communicate;

    for(auto a: vector<int>{0, 1, 2, 3, 4, 0}) {
      Track *curr = world[a], *next = world[curr->getNext()];
      if(curr == world[next->getPrev()]) {
        auto cats = next->freeSlots();
        auto dogs = curr->sendTrains(cats);
        next->addTrains(dogs);
        curr->babystep();
      }
    }
    if(rank == 0) cout << endl;
  }

  if(rank == 0) cout << "timestep " << numSteps <<":" << endl;

  cout << "track " << rank << " (length: " << myTrack->capacity() << ")" 
       << endl << *myTrack << endl << endl;
  }

  MPI_Finalize();
	return 0;
}

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

int main(int argc, char *argv[])
{
	MPI_Init(&argc, &argv);

  int size;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  if(size != 5) {
    cout << "This program only runs with 5 processes. Deal with it. *sunglasses*" << endl;
    MPI_Finalize();
    return 1;
  }

  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  if(rank == 0) {
    if(argc != 5) {
		  cerr << "Format: " << argv[0] 
           << " [trackLength] [starting trains per segment] [trainSpeed] [numSteps]"  << endl;
      MPI_Finalize();
      return 1;
    }
  }

  uint64_t trackLen(stoul(argv[1])), numTrains(stoul(argv[2])), 
           speed(stoul(argv[3])), numSteps(stoul(argv[4]));

  if(rank == 0) {
    if(numTrains > trackLen) {
      cerr << "SHE CANNAE TAKE ANY MORE TRAINS CAP'N (trains per length < trackLen)" << endl;
      MPI_Finalize();
      return 1;
    }

    if(speed > trackLen) {
      cerr << "This isn't FTLT: Faster Than Light Trains (trainSpeed < trackLen)" << endl;
      MPI_Finalize();
      return 1;
    }
  }

  Track* myTrack;

  //center, l, r, t, b
  switch(rank) {
    case 0:
      myTrack = new Intersection(trackLen, 1, 2, 3, 4);
      break;
    case 1:
      myTrack = new Track(trackLen, 2, 0);
      break;
    case 2:
      myTrack = new Track(trackLen, 0, 1);
      break;
    case 3:
      myTrack = new Track(trackLen, 4, 4);
      break;
    case 4:
      myTrack = new Track(trackLen, 3, 3);
      break;
    default:
      cerr << "I\nwhat?\nhow?" << endl;
      MPI_Finalize();
      return 1;
  }
  
  myTrack->randFill(numTrains, speed);

  MPI_Request request;

  for(uint64_t i = 0; i<numSteps; i++) {
    if(rank == 0) cout << "timestep " << i <<":" << endl;
    
    cout << "track " << rank << " (length: " << (myTrack->capacity()) 
         << ")" << endl << *myTrack << endl << endl;

    if(rank == 0 && i%3 == 2) {
      myTrack->turn();
      cout << endl << "TURN" << endl << endl;
    }
    
    myTrack->refresh();
    myTrack->babystep();
    myTrack->communicate(MPI_COMM_WORLD, &request);      
    myTrack->babystep(); 
    myTrack->communicate(MPI_COMM_WORLD, &request);   
    
    if(rank == 0) cout << endl;
  }

  if(rank == 0) cout << "timestep " << numSteps <<":" << endl;

  cout << "track " << rank << " (length: " << myTrack->capacity() << ")" 
       << endl << *myTrack << endl << endl;

  MPI_Finalize();
	return 0;
}

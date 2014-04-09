#include <algorithm>
#include <utility>
#include <vector>
#include <random>
#include <iostream>
#include <mpi.h>

using std::copy;
using std::move;
using std::min;
using std::vector;
using std::default_random_engine;
using std::uniform_int_distribution;

#include "track.h"

Track::Track(const Track &t) {
	*this = t;
}

Track::Track(Track &&t) {
	trackLen = move(t.trackLen);
  prev = move(t.prev);
  next = move(t.next);
	track = move(t.track);
}

Track &Track::operator=(const Track &t) {
	trackLen = t.trackLen;
  prev = t.prev;
  next = t.next;
	copy(t.track.begin(), t.track.end(), track.begin());
	return *this;
}

array<uint32_t, 3> *Track::getTrain(uint32_t loc) {
  for(auto &a: track)
    if(a[0] == loc)
      return &a;
  return nullptr;
}

array<uint32_t, 3> Track::popFront() {
  array<uint32_t, 3> train = track.front();
  track.pop_front();
  return train;
}

bool Track::letThereBeTrain(uint32_t loc, uint32_t baseMove, 
                            uint32_t moveLeft) {
    array<uint32_t, 3> temp{{loc, baseMove, moveLeft}}; 
    for(auto it = track.begin(); it != track.end(); it++)
    {
      if((*it)[0] == loc)
        return false;
      else if((*it)[0] > loc) { 
        track.insert(it, temp);
        return true;
      }
    }
    track.insert(track.end(), temp);
    return true;
}

uint32_t Track::freeSlots() { 
  return trackLen - (track.size() == 0 ? 0 : track.back()[0] + 1); 
}

vector<uint32_t> Track::sendTrains(uint32_t slots) {
  vector<uint32_t> outbound;
  outbound[0] = 0;
  outbound.reserve(trackLen*3+1);
  if(track.size() > 0) {
    for(uint32_t i = slots; i > 0; i--) {
      if(track.front()[2] >= track.front()[0] + i) {
        track.front()[2] -= track.front()[0] + i;
        track.front()[0] = trackLen - i;
        for(auto i : popFront())
          outbound.push_back(i);
        outbound[0]++;
      }
    }
  }
  return outbound;
}

ostream &operator<<(ostream& out, const Track &t) {
  for(auto a = t.track.rbegin(); a != t.track.rend(); a++) {
    out << "<" << (*a)[0] << ", " << (*a)[1] << ", " << (*a)[2] << ">, ";
  }
  return out;
}


void Track::addTrains(vector<uint32_t> inbound) {
  int numTrains = inbound[0];
  for(auto i = inbound.begin()+1; i != inbound.begin()+3*numTrains+1; ++i)
    letThereBeTrain(*i, *(++i), *(++i));
}

void Track::addlComm(MPI_Comm network, MPI_Request* reqAddr) {
  (void) network;
  (void) reqAddr;
}

void Track::communicate(MPI_Comm network, MPI_Request* reqAddr) {
  addlComm(network, reqAddr);

  uint32_t slots = freeSlots();

  MPI_Isend(&slots, 1, MPI_UNSIGNED, getPrev(), 0, network, reqAddr);
  MPI_Recv(&slots, 1, MPI_UNSIGNED, getNext(), 0, network, MPI_STATUS_IGNORE);
  
  vector<uint32_t> trains = sendTrains(slots);
  trains.resere(3*trackLen+1);
      
  MPI_Isend(trains.data(), (trackLen*3+1), MPI_UNSIGNED, getNext(), 0, 
            network, reqAddr);
  MPI_Recv(trains.data(), (trackLen*3+1), MPI_UNSIGNED, getPrev(), 0, 
            network, MPI_STATUS_IGNORE);
            
  addTrains(trains);
}


void Track::babystep() {   
  uint32_t dist;
  for(auto it = track.begin(); it != track.end(); it++) {
    if(it == track.begin()) {
      dist = min(track.front()[0], track.front()[2]);
      track.front()[0] -= dist;
      track.front()[2] -= dist;
    } else {
        dist = min((*it)[0] - (*std::prev(it))[0] - 1 , (*it)[2]);
        (*it)[0] -= dist;
        (*it)[2] -= dist;
    }
  }
}

void Track::randFill(uint32_t numTrains, uint32_t trainSpeed, int seed) {
  if(seed == 0) seed = next^prev; //just throw something together
  default_random_engine dre(seed);
  uniform_int_distribution<uint32_t> trackGen(1, 4), locGen(0, trackLen-1);
  
  for(uint32_t i = 0; i<numTrains; i++) {
    while(!letThereBeTrain(locGen(dre), trainSpeed, trainSpeed));
  }
}

void Track::refresh() {
  for(auto &a: track)
    a[2] = a[1];
}



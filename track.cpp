#include <algorithm>
#include <utility>
#include <iterator>

using std::copy;
using std::move;
using std::min;

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

bool Track::letThereBeTrain(uint32_t loc, uint32_t baseMove, uint32_t moveLeft) {
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

void smalltalk() {

}

void Track::refresh() {
  for(auto &a: track)
    a[2] = a[1];
}

ostream &operator<<(ostream& out, const Track &t) {
  for(auto a = t.track.rbegin(); a != t.track.rend(); a++) {
    out <<(*a)[0] << ", ";
  }
  return out;
}

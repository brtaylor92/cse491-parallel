#include <algorithm>
#include <utility>

using std::copy;
using std::move;

#include "track.h"

Track::Track(const Track &t) {
	*this = t;
}

Track::Track(Track &&t) {
	trackLen = move(t.trackLen);
	track = move(t.track);
}

Track &Track::operator=(const Track &t) {
	trackLen = t.trackLen;
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

ostream &operator<<(ostream& out, const Track &t) {
  for(auto &a: t.track) {
    out << a[0] << ", ";
  }
  return out;
}


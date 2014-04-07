#pragma once
#ifndef _TRACK_H
#define _TRACK_H

#include <array>
#include <list>
#include <iterator>
#include <utility>

using std::array;
using std::list;
using std::pair;
using std::prev;
using std::next;
using std::ostream;

class Track {
public:
	Track() = delete;
	Track(const uint32_t t) : trackLen(t) {};
	Track(const Track &t);
	Track(Track &&t);
	Track &operator=(const Track &t);
	~Track() = default;
  uint32_t size() const { return track.size(); }
  uint32_t capacity() const { return trackLen; }
  bool empty() const { return track.empty(); }
  bool full() const { return (track.size() == trackLen); }
  array<uint32_t, 3> *getTrain(uint32_t loc);
  bool letThereBeTrain(uint32_t loc, uint32_t baseMove, uint32_t moveLeft);
 
  friend ostream &operator<<(ostream& out, const Track &t);

private:
	list<array<uint32_t, 3>> track; //location, move per turn, move points left
	uint32_t trackLen;
};


#endif //_TRACK_H

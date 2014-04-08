#pragma once
#ifndef _TRACK_H
#define _TRACK_H

#include <array>
#include <list>
#include <iterator>
#include <utility>
#include <vector>

using std::array;
using std::list;
using std::pair;
using std::prev;
using std::next;
using std::ostream;
using std::vector;

class Track {
public:
	Track() = delete;
	Track(const uint32_t t, const int prev, const int next) : 
    prev(prev), next(next), trackLen(t) {};
	Track(const Track &t);
	Track(Track &&t);
	virtual Track &operator=(const Track &t);
  ~Track() = default;
  friend ostream &operator<<(ostream& out, const Track &t);
  array<uint32_t, 3> *getTrain(uint32_t loc);
  array<uint32_t, 3> popFront();
  bool full() const { return (track.size() == trackLen); }
  bool empty() const { return track.empty(); }
  bool letThereBeTrain(uint32_t loc, uint32_t baseMove, uint32_t moveLeft);
  int getNext() { return next; }
  int getPrev() { return prev; }
  uint32_t capacity() const { return trackLen; }
  uint32_t freeSlots();
  uint32_t size() const { return track.size(); }
  vector<array<uint32_t, 3>> sendTrains(uint32_t numSlots);
  virtual void turn() {}
  void addTrains(vector<array<uint32_t, 3>> inbound);
  void babystep();
  void refresh();

protected:
  int prev, next;
  list<array<uint32_t, 3>> track; //location, move per turn, move points left
	uint32_t trackLen;
};


#endif //_TRACK_H

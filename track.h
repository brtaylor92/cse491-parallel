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
	Track(const uint32_t t, const int prev, const int next) : trackLen(t), prev(prev), next(next) {};
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
  array<uint32_t, 3> popFront();
  void babystep();
  friend ostream &operator<<(ostream& out, const Track &t);
  void refresh();
  uint32_t freeSlots() { return trackLen - (track.size() == 0 ? 0 : track.back()[0] + 1); }
  vector<array<uint32_t, 3>> sendTrains(uint32_t numSlots);
  void addTrains(vector<array<uint32_t, 3>> inbound);
  int getPrev() { return prev; }
  int getNext() { return next; }
  virtual void swap() {}

protected:
	uint32_t trackLen;
  int prev, next;
  list<array<uint32_t, 3>> track; //location, move per turn, move points left
  };


#endif //_TRACK_H

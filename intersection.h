#include "track.h"

class Intersection : public Track {
  public:
    Intersection(const uint32_t len, const int prev, const int next, 
                 const int altP, const int altN) : Track(len, prev, next), 
                 altPrev(altP), altNext(altN) {}

    Intersection& operator=(Intersection &rhs) {
      trackLen = rhs.trackLen;
      prev = rhs.prev;
      next = rhs.next;
	    copy(rhs.track.begin(), rhs.track.end(), track.begin());
      altPrev = rhs.altPrev;
      altNext = rhs.altNext;
      return *this;
    }

    void turn() {
      int temp = prev;
      prev = altPrev;
      altPrev = temp;

      temp = next;
      next = altNext;
      altNext = temp;
    }

  private:
    int altPrev, altNext;
};

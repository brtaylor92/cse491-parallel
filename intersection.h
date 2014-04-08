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
    
    void addlComm(MPI_Comm network, MPI_Request* reqAddr) {
      uint32_t zeroSlots = freeSlots();
      vector<array<uint32_t, 3>> zeroTrains;
      zeroTrains.reserve(trackLen);
  
      MPI_Isend(&zeroSlots, 1, MPI_UNSIGNED, getAltPrev(), 0, network, reqAddr);
      MPI_Isend(&zeroTrains, sizeof(zeroTrains)+trackLen*(sizeof(array<uint32_t,3>)+3*sizeof(uint32_t)), MPI_BYTE, getAltNext(), 0, network, reqAddr);
    }

    void randFill(uint32_t numTrains, uint32_t trainSpeed, int seed) {
    (void) numTrains;
    (void) trainSpeed;
    (void) seed;
    }

    void turn() {
      int temp = prev;
      prev = altPrev;
      altPrev = temp;

      temp = next;
      next = altNext;
      altNext = temp;
    }

    int getAltPrev() { return altPrev; }
    int getAltNext() { return altNext; }

  private:
    int altPrev, altNext;
};

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
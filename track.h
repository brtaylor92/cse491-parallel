#include <array>
#include <list>
#include <utility>

using std::array;
using std::list;
using std::pair;

class Track {
public:
	Track() = delete;
	Track(const uint32_t t) : trackLen(t) {};
	Track(const Track &t);
	Track(Track &&t);
	Track &operator=(const Track &t);
	~Track() = default;
private:
	list<array<uint32_t, 3>> track;
	uint32_t trackLen;
};
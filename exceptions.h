#include <sstream>
#include <stdexcept>

using std::exception;
using std::ostringstream;

class BadDim : public exception {
public:
	BadDim(long rDim, long cDim) {
		msg << "rDim: " << rDim << ", cDim: " << cDim;
	}

	BadDim(const BadDim &rs) : msg(rs.msg.str()) {}

	const char * what() const throw() override { return msg.str().c_str(); }
	
private:
	ostringstream msg;
};
#ifndef EXCEPTIONS_H_
#define EXCEPTIONS_H_
#include <sstream>
#include <stdexcept>

using std::exception;
using std::ostringstream;

class BadDim : public exception {
public:
  BadDim(int64_t rDim, int64_t cDim) {
    msg << "rDim: " << rDim << ", cDim: " << cDim;
  }

  BadDim(const BadDim &rs) : msg(rs.msg.str()) {}

  const char *what() const throw() override { return msg.str().c_str(); }

private:
  ostringstream msg;
};

class SelfAssign : public exception {
public:
  SelfAssign() {}

  const char *what() const throw() override {
    return "Attempted to multiply or add a matrix into itself";
  }
};

#endif // EXCEPTIONS_H_

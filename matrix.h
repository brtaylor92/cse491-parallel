#ifndef MATRIX_H_
#define MATRIX_H_

#include <algorithm>
#include <chrono>
#include <functional>
#include <initializer_list>
#include <iostream>
#include <iterator>
#include <random>
#include <sstream>
#include <string>
#include <vector>

#include "./exceptions.h"

using std::default_random_engine;
using std::endl;
using std::chrono::duration_cast;
using hrc = std::chrono::high_resolution_clock;
using std::chrono::seconds;
using std::initializer_list;
using std::ostream;
using std::ostream_iterator;
using std::plus;
using std::uniform_real_distribution;
using std::vector;

template <typename T> class Matrix {
public:
  //  Default c'tor
  Matrix() = default;

  //  Explicit single-arg c'tor
  explicit Matrix(const uint32_t &dim) : rDim(dim), cDim(dim), m(cDim * rDim) {}

  //  2-arg square c'tor
  Matrix(const uint32_t &dim, T val)
      : rDim(dim), cDim(dim), m(rDim * cDim, val) {}

  //  3-arg multi-dim c'tor
  Matrix(const uint32_t &r, const uint32_t &c, T val)
      : rDim(r), cDim(c), m(rDim * cDim, val) {}

  //  Initializer list c'tor
  Matrix(initializer_list<initializer_list<T> > init)
      : rDim(init.size()), cDim(init.begin()->size()) {
    auto s = init.begin()->size();
    for (auto i : init)
      if (i.size() != s)
        throw BadDim(rDim, cDim);
    m.reserve(cDim * rDim);
    for (auto i : init)
      copy(i.begin(), i.end(), back_inserter(m));
  }

  //  Move c'tor
  Matrix(Matrix &&rs) : rDim(rs.rDim), cDim(rs.cDim), m(move(rs.m)) {}

  //  Copy c'tor for operator=
  Matrix(const Matrix &rs) : rDim(rs.rDim), cDim(rs.cDim), m(rs.m) {}

  //  Operator= (assignment operator)
  Matrix operator=(Matrix rs) {
    swap(*this, rs);
    return *this;
  }

  //  Destructor
  ~Matrix() = default;

  //  Operator() (index operator)
  T &operator()(const uint32_t &r, const uint32_t &c) {
    //  Make sure we're not dumb
    if (r > rDim || c > cDim)
      throw BadDim(rDim, cDim);
    return m.at(r * cDim + c);
  }

  //  Operator<<
  template <typename S>
  friend ostream &operator<<(ostream &o, const Matrix<S> &rs) {
    for (auto i = rs.m.begin(); i != rs.m.end(); i += rs.cDim) {
      copy(i, i + rs.cDim, ostream_iterator<T>(o, ", "));
      o << endl;
    }
    return o;
  }

  //  Add a and b
  void add(const Matrix &a, const Matrix &b) {
    if (a.rDim != rDim || b.rDim != rDim)
      throw BadDim(rDim, cDim);
    if (a.cDim != cDim || b.cDim != cDim)
      throw BadDim(rDim, cDim);
    transform(a.m.begin(), a.m.end(), b.m.begin(), m.begin(), plus<T>());
  }

  //  Multiply a and b
  void mult(const Matrix &a, const Matrix &b) {
    if (this == &a || this == &b || &a == &b)
      return;
    if (a.cDim != b.rDim)
      throw BadDim(a.cDim, b.rDim);
    if (rDim != a.rDim || cDim != b.cDim)
      throw BadDim(rDim, cDim);
    fill(m.begin(), m.end(), 0);
    for (uint32_t r = 0; r < a.rDim; ++r)
      for (uint32_t i = 0; i < a.cDim; ++i)
        for (uint32_t c = 0; c < b.cDim; ++c)
          m[r * cDim + c] += a.m[r * a.cDim + i] * b.m[i * b.cDim + c];
  }

  //  Populate a matrix with random values between min and max
  void Rand(T min, T max, uint32_t seed = 0) {
    default_random_engine r(
        seed ? seed
             : duration_cast<seconds>(hrc::now().time_since_epoch()).count());
    uniform_real_distribution<double> d(min, max);
    auto rng = bind(d, ref(r));
    generate(m.begin(), m.end(), rng);
  }

private:
  const uint32_t rDim; //  Row dimension
  const uint32_t cDim; //  Column dimension
  vector<T> m;         //  Data storage - cDim*r uint32_t
};


// Add or multiply 2 matrices and return the result as a new matrix
template<typename T> Matrix<T> &add(const Matrix<T> &a, const Matrix<T> &b) {
  Matrix<T> c(a.cDim, a.rDim);
  c.add(a, b);
  return c;
}

template<typename T> Matrix<T> &mult(const Matrix<T> &a, const Matrix<T> &b) {
  Matrix<T> c(a.rDim, b.cDim);
  c.mult(a, b);
  return c;
}

#endif // MATRIX_H_

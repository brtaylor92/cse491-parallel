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
  Matrix &operator=(Matrix rs) {
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

  //  Add a to b into c
  template <typename S>
  friend Matrix<S> &add(const Matrix<S> &a, const Matrix<S> &b, Matrix<S> &c);

  //  Mult a by b into c
  template <typename S>
  friend Matrix<S> &mult(const Matrix<S> &a, const Matrix<S> &b, Matrix<S> &c);

  //  Operator<<
  template <typename S>
  friend ostream &operator<<(ostream &o, const Matrix<S> &rs) {
    for (auto i = rs.m.begin(); i != rs.m.end(); i += rs.cDim) {
      copy(i, i + rs.cDim, ostream_iterator<T>(o, ", "));
      o << endl;
    }
    return o;
  }

  //  Populate a matrix with random values between min and max
  void rand(T min, T max, uint32_t seed = 0) {
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

//  Add a and b, return the result
template <typename T>
Matrix<T> &add(const Matrix<T> &a, const Matrix<T> &b, Matrix<T> &t) {
  if (a.rDim != t.rDim || b.rDim != t.rDim)
    throw BadDim(t.rDim, t.cDim);
  if (a.cDim != t.cDim || b.cDim != t.cDim)
    throw BadDim(t.rDim, t.cDim);
  transform(a.m.begin(), a.m.end(), b.m.begin(), t.m.begin(), plus<T>());
  return t;
}

//  Add or multiply 2 matrices and return the result as a new matrix
template <typename T> Matrix<T> &add(const Matrix<T> &a, const Matrix<T> &b) {
  Matrix<T> c(a.cDim, a.rDim);
  add(a, b, c);
  return c;
}

//  Multiply a and b into c
template <typename T>
Matrix<T> &mult(const Matrix<T> &a, const Matrix<T> &b, Matrix<T> &t) {
  if (a.cDim != b.rDim)
    throw BadDim(a.cDim, b.rDim);
  if (t.rDim != a.rDim || t.cDim != b.cDim)
    throw BadDim(t.rDim, t.cDim);
  fill(t.m.begin(), t.m.end(), 0);
  for (uint32_t r = 0; r < a.rDim; ++r)
    for (uint32_t i = 0; i < a.cDim; ++i)
      for (uint32_t c = 0; c < b.cDim; ++c)
        t.m[r * t.cDim + c] += a.m[r * a.cDim + i] * b.m[i * b.cDim + c];
  return t;
}

//  Multiply a by b, return the result
template <typename T> Matrix<T> &mult(const Matrix<T> &a, const Matrix<T> &b) {
  Matrix<T> c(a.rDim, b.cDim);
  mult(a, b, c);
  return c;
}

#endif // MATRIX_H_

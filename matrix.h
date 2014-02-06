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
#include <type_traits>
#include <vector>

#include "./exceptions.h"

using std::default_random_engine;
using std::endl;
using std::chrono::duration_cast;
using hrc = std::chrono::high_resolution_clock;
using std::chrono::seconds;
using std::initializer_list;
using std::is_arithmetic;
using std::multiplies;
using std::ostream;
using std::ostream_iterator;
using std::placeholders::_1;
using std::plus;
using std::seed_seq;
using std::swap;
using std::uniform_real_distribution;
using std::vector;

template <typename T> class Matrix {
public:
  //  Default c'tor
  Matrix() = default;

  //  Explicit single-arg c'tor
  explicit Matrix(const uint32_t &dim) : rDim(dim), cDim(dim), m(rDim * cDim) {
    if (!is_arithmetic<T>::value)
      throw BadType();
  }

  //  2-arg square c'tor
  Matrix(const uint32_t &dim, T val)
      : rDim(dim), cDim(dim), m(rDim * cDim, val) {
    if (!is_arithmetic<T>::value)
      throw BadType();
  }

  //  3-arg multi-dim c'tor
  Matrix(const uint32_t &rows, const uint32_t &cols, T val)
      : rDim(rows), cDim(cols), m(rDim * cDim, val) {
    if (!is_arithmetic<T>::value)
      throw BadType();
  }

  //  Initializer list c'tor
  Matrix(initializer_list<initializer_list<T> > init)
      : rDim(init.size()), cDim(init.begin()->size()) {
    if (!is_arithmetic<T>::value)
      throw BadType();
    auto s = init.begin()->size();
    for (const auto &i : init)
      if (i.size() != s)
        throw BadDim(rDim, cDim);
    m.reserve(cDim * rDim);
    for (auto i : init)
      copy(i.begin(), i.end(), back_inserter(m));
  }

  //  Move c'tor
  Matrix(Matrix &&rs) : rDim(rs.rDim), cDim(rs.cDim) {
    swap(m, rs.m);
    rs.m.clear();
    rs.rDim = 0;
    rs.cDim = 0;
  }

  //  Copy c'tor for operator=
  Matrix(const Matrix &rs) : rDim(rs.rDim), cDim(rs.cDim), m(rs.m) {}

  //  Operator= (assignment operator)
  Matrix &operator=(const Matrix &rs) {
    m = rs.m;
    rDim = rs.rDim;
    cDim = rs.cDim;
    return *this;
  }

  //  Destructor
  ~Matrix() = default;

  //  Operator() (index operator)
  T &operator()(const uint32_t &rows, const uint32_t &cols) {
    //  Make sure we're not dumb
    if (rows > rDim || cols > cDim)
      throw BadDim(rDim, cDim);
    return m.at(rows * cDim + cols);
  }

  //  Get dimensions
  inline uint32_t rows() const { return rDim; }

  inline uint32_t cols() const { return cDim; }

  bool operator==(const Matrix &rhs) const {
    if (m.size() != rhs.m.size())
      return false;
    for (auto i = 0; i < m.size(); i++)
      if (m.at(i) != rhs.m.at(i))
        return false;
    return true;
  }

  //  Add a to b into t
  Matrix &add(const Matrix &b, Matrix &t) const {
    if (this == &t || &b == &t)
      throw SelfAssign();
    if (rDim != t.rDim || b.rDim != t.rDim)
      throw BadDim(t.rDim, t.cDim);
    if (cDim != t.cDim || b.cDim != t.cDim)
      throw BadDim(t.rDim, t.cDim);
    transform(m.begin(), m.end(), b.m.begin(), t.m.begin(), plus<T>());
    return t;
  }

  //  Mult a by b into t
  Matrix &mult(const Matrix &b, Matrix &t) const {
    if (this == &t || &b == &t)
      throw SelfAssign();
    if (cDim != b.rDim)
      throw BadDim(cDim, b.rDim);
    if (t.rDim != rDim || t.cDim != b.cDim)
      throw BadDim(t.rDim, t.cDim);
    fill(t.m.begin(), t.m.end(), 0);
    for (uint32_t r = 0; r < rDim; ++r)
      for (uint32_t i = 0; i < cDim; ++i)
        for (uint32_t c = 0; c < b.cDim; ++c)
          t.m[r * t.cDim + c] += m[r * cDim + i] * b.m[i * b.cDim + c];
    return t;
  }

  //  Mult t by a scalar s
  Matrix &mult(const T &s, Matrix &t) const {
    if (cDim != t.cDim || rDim != t.rDim)
      throw BadDim(t.rDim, t.cDim);
    transform(m.begin(), m.end(), t.m.begin(), bind(multiplies<T>(), _1, s));
    return t;
  }

  template <typename S>
  friend ostream &operator<<(ostream &o, const Matrix<S> &rs);

  //  Populate a matrix with random values between min and max
  void rand(T min, T max, uint32_t seed = 0) {
    seed_seq s{ seed, static_cast<uint32_t>(
                          duration_cast<seconds>(hrc::now().time_since_epoch())
                              .count()) };
    default_random_engine r(s);
    uniform_real_distribution<double> d(min, max);
    auto rng = bind(d, ref(r));
    generate(m.begin(), m.end(), rng);
  }

private:
  uint32_t rDim; //  Row dimension
  uint32_t cDim; //  Column dimension
  vector<T> m;   //  Data storage - cDim*r uint32_t
};

//  Add or multiply 2 matrices and return the result as a new matrix
template <typename T> Matrix<T> add(const Matrix<T> &a, const Matrix<T> &b) {
  Matrix<T> c(a.rows(), a.cols(), 0);
  a.add(b, c);
  return c;
}

//  Multiply a by b, return the result
template <typename T> Matrix<T> mult(const Matrix<T> &a, const Matrix<T> &b) {
  Matrix<T> c(a.rows(), b.cols(), 0);
  a.mult(b, c);
  return c;
}

//  Multiply a by a scalar s, return the result
template <typename T> Matrix<T> mult(const T &s, const Matrix<T> &a) {
  Matrix<T> c(a.rows(), a.cols(), 0);
  a.mult(s, c);
  return c;
}

//  Operator<<
template <typename T> ostream &operator<<(ostream &o, const Matrix<T> &rs) {
  for (auto i = rs.m.begin(); i != rs.m.end(); i += rs.cDim) {
    copy(i, i + rs.cDim - 1, ostream_iterator<T>(o, ", "));
    o << *(i + rs.cDim - 1) << endl;
  }
  return o;
}

#endif // MATRIX_H_
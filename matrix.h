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
  explicit Matrix(const uint32_t &dim) : r(dim), c(dim), m(rDim * cDim) {
    if (!is_arithmetic<T>::value)
      throw BadType();
  }

  //  2-arg square c'tor
  Matrix(const uint32_t &dim, T val) : r(dim), c(dim), m(rDim * cDim, val) {
    if (!is_arithmetic<T>::value)
      throw BadType();
  }

  //  3-arg multi-dim c'tor
  Matrix(const uint32_t &rows, const uint32_t &cols, T val)
      : r(rows), c(cols), m(rDim * cDim, val) {
    if (!is_arithmetic<T>::value)
      throw BadType();
  }

  //  Initializer list c'tor
  Matrix(initializer_list<initializer_list<T> > init)
      : r(init.size()), c(init.begin()->size()) {
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
  Matrix(Matrix &&rs) : r(rs.rDim), c(rs.cDim) {
    swap(m, rs.m);
    rs.m.clear();
    rs.r = 0;
    rs.c = 0;
  }

  //  Copy c'tor for operator=
  Matrix(const Matrix &rs) : r(rs.rDim), c(rs.cDim), m(rs.m) {}

  //  Operator= (assignment operator)
  Matrix &operator=(const Matrix &rs) {
    m = rs.m;
    r = rs.rDim;
    c = rs.cDim;
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
    if(m.size() != rhs.m.size())
      return false;
    for(auto i = 0; i < m.size(); i++)
      if(m.at(i) != rhs.m.at(i))
        return false;
    return true;
  }

  //  Add a to b into t
  static Matrix &add(const Matrix &a, const Matrix &b, Matrix &t) {
    if (&a == &t || &b == &t)
      throw SelfAssign();
    if (a.rDim != t.rDim || b.rDim != t.rDim)
      throw BadDim(t.rDim, t.cDim);
    if (a.cDim != t.cDim || b.cDim != t.cDim)
      throw BadDim(t.rDim, t.cDim);
    transform(a.m.begin(), a.m.end(), b.m.begin(), t.m.begin(), plus<T>());
    return t;
  }

  //  Mult a by b into t
  static Matrix &mult(const Matrix &a, const Matrix &b, Matrix &t) {
    if (&a == &t || &b == &t)
      throw SelfAssign();
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

  //  Mult t by a scalar s
  static Matrix &mult(const T &s, const Matrix &a, Matrix &t) {
    if (a.cDim != t.cDim || a.rDim != t.rDim)
      throw BadDim(t.rDim, t.cDim);
    transform(a.m.begin(), a.m.end(), t.m.begin(),
              bind(multiplies<T>(), _1, s));
    return t;
  }

  template <typename S>
  friend ostream &operator<<(ostream &o, const Matrix<S> &rs);

  //  Populate a matrix with random values between min and max
  void rand(T min, T max, uint32_t seed = 0) {
    seed_seq s{ seed,
                static_cast<uint32_t>(duration_cast<seconds>(hrc::now().time_since_epoch()).count()) };
    default_random_engine r(s);
    uniform_real_distribution<double> d(min, max);
    auto rng = bind(d, ref(r));
    generate(m.begin(), m.end(), rng);
  }

private:
  uint32_t r;
  uint32_t c;
  const uint32_t &rDim = r; //  Row dimension
  const uint32_t &cDim = c; //  Column dimension
  vector<T> m;              //  Data storage - cDim*r uint32_t
};

//  Add or multiply 2 matrices and return the result as a new matrix
template <typename T> Matrix<T> add(const Matrix<T> &a, const Matrix<T> &b) {
  Matrix<T> c(a.rows(), a.cols(), 0);
  Matrix<T>::add(a, b, c);
  return c;
}

//  Multiply a by b, return the result
template <typename T> Matrix<T> mult(const Matrix<T> &a, const Matrix<T> &b) {
  Matrix<T> c(a.rows(), b.cols(), 0);
  Matrix<T>::mult(a, b, c);
  return c;
}

//  Multiply a by a scalar s, return the result
template <typename T> Matrix<T> mult(const T &s, const Matrix<T> &a) {
  Matrix<T> c(a.rows(), a.cols(), 0);
  Matrix<T>::mult(s, a, c);
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
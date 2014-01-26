#ifndef MATRIX_H_
#define MATRIX_H_

// Copyright for google lint purposes
// Copyright 2014 Ben Taylor and Martin Scherr

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
  explicit Matrix(const int32_t &dim) : rDim(dim), cDim(dim), m(cDim * rDim) {
    if (dim <= 0)
      throw BadDim(rDim, cDim);
  }

  //  2-arg square c'tor
  Matrix(const int32_t &dim, T val)
      : rDim(dim), cDim(dim), m(rDim * cDim, val) {
    if (dim <= 0)
      throw BadDim(rDim, cDim);
  }

  //  3-arg multi-dim c'tor
  Matrix(const int32_t &r, const int32_t &c, T val)
      : rDim(r), cDim(c), m(rDim * cDim, val) {
    if (rDim <= 0 || cDim <= 0)
      throw BadDim(rDim, cDim);
  }

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
  T &operator()(const int32_t &r, const int32_t &c) {
    //  Make sure we're not dumb
    if (r > rDim || c > cDim)
      throw BadDim(rDim, cDim);
    //  Make sure the user knows they're dumb
    if (r < 0 || c < 0)
      throw BadDim(r, c);
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

  //  Add a and b into *this
  void add(const Matrix &a, const Matrix &b) {
    if (a.rDim != rDim || b.rDim != rDim)
      throw BadDim(rDim, cDim);
    if (a.cDim != cDim || b.cDim != cDim)
      throw BadDim(rDim, cDim);
    transform(a.m.begin(), a.m.end(), b.m.begin(), m.begin(), plus<T>());
  }

  friend Matrix &add(const Matrix &a, const Matrix &b) {
    Matrix c(a.cDim, a.rDim);
    c.add(a, b);
    return c;
  }

  //  Multiply a and b into c (or return for assignment)
  void mult(const Matrix &a, const Matrix &b) {
    if (this == &a || this == &b || &a == &b)
      return;
    if (a.cDim != b.rDim)
      throw BadDim(a.cDim, b.rDim);
    if (rDim != a.rDim || cDim != b.cDim)
      throw BadDim(rDim, cDim);
    fill(m.begin(), m.end(), 0);
    for (auto r = 0; r < a.rDim; ++r)
      for (auto i = 0; i < a.cDim; ++i)
        for (auto c = 0; c < b.cDim; ++c)
          m[r * cDim + c] += a.m[r * a.cDim + i] * b.m[i * b.cDim + c];
  }

  friend Matrix &mult(const Matrix &a, const Matrix &b) {
    Matrix c(a.rDim, b.cDim);
    c.mult(a, b);
    return c;
  }

  //  Populate a matrix with random values between min and max
  void Rand(T min, T max, int32_t seed = 0) {
    default_random_engine r(
        seed ? seed
             : duration_cast<seconds>(hrc::now().time_since_epoch()).count());
    uniform_real_distribution<double> d(min, max);
    auto rng = bind(d, ref(r));
    generate(m.begin(), m.end(), rng);
  }

private:
  const int32_t rDim;   //  Row dimension
  const int32_t cDim;   //  Column dimension
  vector<T> m;          //  Data storage - cDim*r int32_t
};

#endif  // MATRIX_H_

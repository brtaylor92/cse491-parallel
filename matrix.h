#ifndef MATRIX_H_
#define MATRIX_H_

#include <algorithm>
#include <atomic>
#include <chrono>
#include <functional>
#include <initializer_list>
#include <iostream>
#include <iterator>
#include <random>
#include <sstream>
#include <string>
#include <thread>
#include <type_traits>
#include <vector>
#include <cmath>
#include <cstdio>

#include "./exceptions.h"
#include "./tQueue.h"

using std::atomic_fetch_sub;
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
using std::sort;
using std::thread;
using std::uniform_real_distribution;
using std::vector;
using std::max_element;

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
    for (typename vector<T>::size_type i = 0; i < m.size(); i++)
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

  Matrix &tAdd(const Matrix &b, Matrix &t, uint32_t n = 1) const {
    if (this == &t || &b == &t)
      throw SelfAssign();
    if (rDim != t.rDim || b.rDim != t.rDim)
      throw BadDim(t.rDim, t.cDim);
    if (cDim != t.cDim || b.cDim != t.cDim)
      throw BadDim(t.rDim, t.cDim);
    auto f = [&](uint32_t i) {
      transform(m.begin() + i * m.size() / n,
                m.begin() + (i + 1) * m.size() / n,
                b.m.begin() + i * m.size() / n, t.m.begin() + i * m.size() / n,
                plus<T>());
    };

    vector<thread> threads(n - 1);
    for (uint32_t i = 0; i < n - 1; i++) {
      threads[i] = thread(f, i);
    }
    f(n - 1);
    for (auto &i : threads)
      i.join();
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

  Matrix &tMult(const Matrix &b, Matrix &t, uint32_t n = 1) const {
    if (this == &t || &b == &t)
      throw SelfAssign();
    if (cDim != b.rDim)
      throw BadDim(cDim, b.rDim);
    if (t.rDim != rDim || t.cDim != b.cDim)
      throw BadDim(t.rDim, t.cDim);
    fill(t.m.begin(), t.m.end(), 0);

    auto f = [&](uint32_t i) {
      for (uint32_t r = i * rDim / n; r < (i + 1) * rDim / n; ++r)
        for (uint32_t i = 0; i < cDim; ++i)
          for (uint32_t c = 0; c < b.cDim; ++c)
            t.m[r * t.cDim + c] += m[r * cDim + i] * b.m[i * b.cDim + c];
    };

    vector<thread> threads(n - 1);
    for (uint32_t i = 0; i < n - 1; i++)
      threads[i] = thread(f, i);
    f(n - 1);
    for (auto &i : threads)
      i.join();
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

  void tRand(T min, T max, uint32_t seed = 0, uint32_t n = 0) {
    seed_seq s{ seed, static_cast<uint32_t>(
                          duration_cast<seconds>(hrc::now().time_since_epoch())
                              .count()) };
    default_random_engine r(s);
    uniform_real_distribution<double> d(min, max);
    auto f = [&](uint32_t i) {
      for (auto j = m.begin() + i * m.size() / n;
           j != m.begin() + (i + 1) * m.size() / n; j++) {
        *j = d(r);
      }
    };

    vector<thread> t(n - 1);
    for (uint32_t i = 0; i < n - 1; i++) {
      t[i] = thread(f, i);
    }
    f(n - 1);
    for (auto &i : t)
      i.join();
  }

  void shearSort() {
    // convert the matrix to a square by inserting filler zeroes
    uint32_t sqDim;
    if (rows() != cols()) {
      sqDim = uint32_t(ceil(sqrt(rows() * cols())));
      m.resize(sqDim * sqDim, *max_element(m.begin(), m.end()));
    } else {
      sqDim = rows();
    }

    // shearsort phases
    for (auto phasecount = 0; phasecount < log(sqDim); phasecount++) {

      // sort rows snakewise; note the xor in the custom comp function
      for (uint32_t i = 0; i < sqDim; i++) {
        sort(m.begin() + i * sqDim, m.begin() + (i + 1) * sqDim,
             [&](T a, T b) { return (a < b) ^ (i % 2); });
      }

      // sort columns (copies to a vector, sorts there, copies back)
      vector<T> col_refs;
      for (auto i = m.begin(); i != m.begin() + sqDim; i++) {
        for (auto j = i; j < m.end(); j += sqDim) {
          col_refs.push_back(*j);
        }
        sort(col_refs.begin(), col_refs.end());
        for (uint32_t j = 0; j < sqDim; j++) {
          *(i + j *sqDim) = col_refs[j];
        }
        col_refs.clear();
      }
    }

    // last phase of sort, leaves data in regular sort order
    for (uint32_t i = 0; i < sqDim; i++) {
      sort(m.begin() + i * sqDim, m.begin() + (i + 1) * sqDim);
    }

    // delete filler data
    if (rows() != cols())
      m.resize(rows() * cols());
  }

  void tShearSort(uint32_t numThreads) {
    vector<thread> threadPool(numThreads);
    atomic<int> workLeft(numThreads), sync(0);
    uint32_t sqDim;
    if (rows() != cols()) {
      sqDim = uint32_t(ceil(sqrt(rows() * cols())));
      m.resize(sqDim * sqDim, *max_element(m.begin(), m.end()));
    } else {
      sqDim = rows();
    }
    uint32_t phaseCount = log(sqDim)+3;

    TQueue<uint32_t> q;
    for(uint32_t i = 0; i < sqDim; i++)
      q.push(i);

    for(uint32_t i = 0; i < numThreads; i++)
      threadPool[i] = thread(&Matrix::tInnerSort, this, std::ref(q), sqDim, numThreads, std::ref(workLeft), 
                             std::ref(phaseCount), i, std::ref(sync));
    for(auto &i : threadPool) {
      i.join();
    }
    // delete filler data
    if (rows() != cols())
      m.resize(rows() * cols());
  }

private:
  uint32_t rDim; //  Row dimension
  uint32_t cDim; //  Column dimension
  vector<T> m;   //  Data storage - cDim*r

  void tInnerSort(TQueue<uint32_t> &q, const uint32_t sqDim, const uint32_t numThreads,
                  atomic<int> &workLeft, uint32_t &phaseCount, const uint32_t tid, atomic<int> &sync) {
    uint32_t row;
    while(phaseCount > 0) {
      if(phaseCount != 1) {
        while(q.pop(row)) {
          sort(m.begin() + row * sqDim, m.begin() + (row + 1) * sqDim,
               [&](T a, T b) { return (a < b) ^ (row % 2); });
        }

        atomic_fetch_sub(&workLeft, 1);
        while(workLeft > 0);
        if(tid == 0) {
          for (uint32_t i = 0; i < sqDim; i++)
            q.push(i);
        }

        //synchronize threads  
        atomic_fetch_add(&sync, 1);
        while(sync != numThreads);

        vector<T> col_refs;
        while(q.pop(row)) {
          for (auto j = m.begin() + row; j < m.end(); j += sqDim) {
            col_refs.push_back(*j);
          }
          sort(col_refs.begin(), col_refs.end());
          for (uint32_t j = 0; j < sqDim; j++) {
            *(j * sqDim + (row+m.begin())) = col_refs[j];
          }
          col_refs.clear();
        }
        atomic_fetch_add(&workLeft, 1);
        while(workLeft != numThreads);
        if(tid == 0) {
          for (uint32_t i = 0; i < sqDim; i++)
            q.push(i);
          phaseCount--;
        }
        //synchronize threads
        atomic_fetch_sub(&sync, 1);
        while(sync != 0);
      } else {
          while(q.pop(row)) {
            sort(m.begin() + row * sqDim, m.begin() + (row + 1) * sqDim);
          }
          atomic_fetch_sub(&workLeft, 1);
          while(workLeft > 0);
          if(tid == 0) {
            phaseCount--;
          }
           //synchronize threads
          atomic_fetch_add(&sync, 1);
          while(sync != numThreads);
      }
      
    }
  }
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

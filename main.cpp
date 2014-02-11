#include "./matrix.h"

using std::cout;
using std::chrono::microseconds;
using std::chrono::milliseconds;
using std::endl;
using std::move;

void tBigAdd(int t) {
  constexpr int sz = 10000;
  using T = double;
  Matrix<T> d1(sz), d2(sz), d3(sz);
  d1.tRand(0, 1, 13, t);
  d2.tRand(0, 1, 16, t);
  d1.tAdd(d2, d3, t);
}

void bigAdd() {
  constexpr int sz = 10000;
  constexpr int t = 16;
  using T = double;
  Matrix<T> d1(sz), d2(sz), d3(sz);
  d1.tRand(0, 1, 13, t);
  d2.tRand(0, 1, 16, t);
  d1.add(d2, d3);
}

void tBigMult(int t) {
  constexpr int sz = 1000;
  using T = double;
  Matrix<T> d1(sz), d2(sz), d3(sz);
  d1.tRand(0, 1, 13, t);
  d2.tRand(0, 1, 16, t);
  d1.tMult(d2, d3, t);
}


void randFill() {
  using T = double;
  constexpr int sz = 1000;
  Matrix<T> d1(sz), d2(sz), d3(sz);
  d1.rand(0,1,10);
  d2.rand(0,1,12);
  d3.rand(0,1,14);
}

void tBigRand(int t) {
  using T = double;
  constexpr int sz = 1000;
  Matrix<T> d1(sz), d2(sz), d3(sz);
  d1.tRand(0,1,10,t);
  d2.tRand(0,1,12,t);
  d3.tRand(0,1,14,t);
}

int main(int argc, char const *argv[]) {
  (void)argc;
  (void)argv;

  // Test constructors
  Matrix<int64_t> a(4);
  Matrix<int64_t> b(4, 7);
  Matrix<int64_t> c(3, 2, 13);
  Matrix<int64_t> d{ { 1, 2, 3 }, { 4, 5, 6 }, { 7, 8, 9 } };

  cout << "a: " << endl << a << endl << "b: " << endl << b << endl
       << "c: " << endl << c << endl << "d: " << endl << d << endl;

  Matrix<int64_t> e = move(d);

  cout << "d: " << endl << d << endl << "d dimensions: " << endl << d.rows()
       << endl << d.cols() << endl << "e: " << endl << e << endl;

  d = e;

  cout << "d: " << endl << d << endl;

  // Test operator()
  cout << "e(1,2): " << e(1, 2) << endl << "e(2,1) = 0" << endl;
  e(2, 1) = 0;
  cout << "e: " << endl << e << endl;

  // Edge cases...
  Matrix<int64_t> f{ {} };
  cout << "f: " << f << endl;

  // Copy constructor
  Matrix<int64_t> g(c);
  cout << "g: " << endl << g << endl;

  Matrix<int64_t> h = b;
  cout << "h: " << endl << h << endl;

  Matrix<int64_t> i(3, 2, 0);
  Matrix<int64_t> j{ { 1, 3 }, { 7, 11 }, { 5, 19 } };
  c.tAdd(j, i, 8);
  cout << add(c, c) << endl;
  cout << "i = c+j: " << endl << i << endl;
  cout << "c+j: " << endl << add(c, j) << endl;

  Matrix<int64_t> k(3, 2, 0);
  e.mult(c, k);
  cout << "k = e*c" << endl << "e: " << endl << e << endl << "c: " << endl << c
       << endl << "k: " << endl << k << endl;
  cout << "e*c: " << endl << mult<int64_t>(e, c) << endl;

  Matrix<int64_t> l(3, 2, 0);
  k.mult(3, l);
  cout << "l = 3*k: " << endl << "l: " << endl << l << endl;
  cout << "3*k: " << endl << mult(static_cast<int64_t>(3), k) << endl;

  Matrix<int64_t> m(4, 0);
  m.rand(0, 10);
  cout << "m: " << endl << m << endl;

  microseconds d1{ 0 }, d2{ 0 };

  /*for (int i = 0; i < 10; i++) {
    auto t1 = hrc::now();
    bigAdd(); 
    // randFill();
    auto t2 = hrc::now();

    dd += duration_cast<microseconds>(t2 - t1);
  }

  dd /= 10;

  cout << "Took " << duration_cast<milliseconds>(dd).count() << "ms ("
       << static_cast<double>(dd.count()) / 1000000 << "s)" << endl;

  dd -= dd;
*/
  
  //parallelism testing
  int threadcount = 16;
 /* constexpr int sz = 100;
  using T = double;
  Matrix<T> m1(sz), m2(sz), m3(sz);
  m1.tRand(0, 1, 13, threadcount);
  m2.tRand(0, 1, 16, threadcount);
  m1.tMult(m2, m3, threadcount);
  cout << "correctness test " << (m3 == mult<T>(m1,m2) ? "passed" : "failed") << endl;
*/

  //single thread timing
  for (int i = 0; i < 10; i++) {
    auto t1 = hrc::now();
    tBigRand(1);
    auto t2 = hrc::now();

    d1 += duration_cast<microseconds>(t2 - t1);
  }
  d1 /= 10;

  //multithread timing
  for (int i = 0; i < 10; i++) {
    auto t1 = hrc::now();
    tBigRand(threadcount);
    auto t2 = hrc::now();

    d2 += duration_cast<microseconds>(t2 - t1);
  }
  d2 /= 10;

  //parallelism outputs
  cout << "1 thread took " << duration_cast<milliseconds>(d1).count() << "ms ("
       << static_cast<double>(d1.count()) / 1000000 << "s)" << endl
       << threadcount << " threads took " << duration_cast<milliseconds>(d2).count() << "ms ("
       << static_cast<double>(d2.count()) / 1000000 << "s)" << endl
       << "Speedup: " << 1.0*d1/d2 
       << "    (fraction of linear: " << 1.0*d1/(d2*threadcount) << ")" << endl << endl;

  Matrix<int64_t> t1(3, 3, 3), t2 = t1, t3(3,3,2), t4(3,2,3);

  cout << "t1 == t2: " << (t1==t2?"yes":"no") << endl;
  cout << "t1 == t3: " << (t1==t3?"yes":"no") << endl;
  cout << "t1 == t4: " << (t1==t4?"yes":"no") << endl;

  Matrix<int64_t> x(3,3,0);
  cout << x << endl;

  x.tRand(3, 13, 7, 8);
  cout << x << endl;

  return 0;
}

#include "matrix.h"

using std::cout;
using std::chrono::microseconds;
using std::chrono::milliseconds;
using std::endl;
using std::move;

void bigMult() {
  constexpr int sz = 500;
  Matrix<double> d1(sz), d2(sz), d3(sz);
  d1.rand(0, 1, 13);
  d2.rand(0, 1, 16);
  d3.mult(d1, d2);
}

int main(int argc, char const *argv[]) {
  (void)argc;
  (void)argv;

  // Test constructors
  Matrix<long> a(4);
  Matrix<long> b(4, 7);
  Matrix<long> c(3, 2, 13);
  Matrix<long> d{ { 1, 2, 3 }, { 4, 5, 6 }, { 7, 8, 9 } };

  cout << "a: " << endl << a << endl << "b: " << endl << b << endl
       << "c: " << endl << c << endl << "d: " << endl << d << endl;

  Matrix<long> e = move(d);

  cout << "d: " << endl << d << endl << "e: " << endl << e << endl;

  // Test operator()
  cout << "e(1,2): " << e(1, 2) << endl << "e(2,1) = 0" << endl;
  e(2, 1) = 0;
  cout << "e: " << endl << e << endl;

  // Edge cases...
  Matrix<long> f{ {} };
  cout << "f: " << f << endl;

  // Copy constructor
  Matrix<long> g(c);
  cout << "g: " << endl << g << endl;

  Matrix<long> h = b;
  cout << "h: " << endl << h << endl;

  Matrix<long> i(3, 2, 0);
  Matrix<long> j{ { 1, 3 }, { 7, 11 }, { 5, 19 } };
  i.add(c, j);
  cout << "i: " << endl << i << endl;

  Matrix<long> k(3, 2, 0);
  k.mult(e, c);
  cout << "k = e*c" << endl << "e: " << endl << e << endl << "c: " << endl << c
       << endl << "k: " << endl << k << endl;

  Matrix<long> l(4, 0);
  l.rand(0, 10);
  cout << "l: " << endl << l << endl;

  microseconds dd{ 0 };

  for (int i = 0; i < 10; i++) {
    auto t1 = hrc::now();
    bigMult();
    auto t2 = hrc::now();

    dd += duration_cast<microseconds>(t2 - t1);
  }

  dd /= 10;

  cout << "Took " << duration_cast<milliseconds>(dd).count() << "ms ("
       << static_cast<double>(dd.count()) / 1000000 << "s)" << endl;

  return 0;
}
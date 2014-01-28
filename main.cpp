#include "./matrix.h"

using std::cout;
using std::chrono::microseconds;
using std::chrono::milliseconds;
using std::endl;
using std::move;

void bigMult() {
  constexpr int sz = 500;
  using T = int64_t;
  Matrix<T> d1(sz), d2(sz), d3(sz);
  d1.rand(0, 1, 13);
  d2.rand(0, 1, 16);
  Matrix<T>::mult(d1, d2, d3);
}

int main(int argc, char const *argv[]) {
  (void)argc;
  (void)argv;

  /*// Test constructors
  Matrix<int64_t> a(4);
  Matrix<int64_t> b(4, 7);
  Matrix<int64_t> c(3, 2, 13);
  Matrix<int64_t> d{ { 1, 2, 3 }, { 4, 5, 6 }, { 7, 8, 9 } };

  cout << "a: " << endl << a << endl << "b: " << endl << b << endl
       << "c: " << endl << c << endl << "d: " << endl << d << endl;

  Matrix<int64_t> e = move(d);

  cout << "d: " << endl << d << endl << "e: " << endl << e << endl;

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
  Matrix<int64_t>::add(c, j, i);
  cout << "i = c+j: " << endl << i << endl;
  cout << "c+j: " << endl << add<int64_t>(c, j) << endl;

  Matrix<int64_t> k(3, 2, 0);
  Matrix<int64_t>::mult(e, c, k);
  cout << "k = e*c" << endl << "e: " << endl << e << endl << "c: " << endl << c
       << endl << "k: " << endl << k << endl;
  cout << "e*c: " << endl << mult<int64_t>(e, c) << endl;

  Matrix<int64_t> l(3, 2, 0);
  Matrix<int64_t>::mult(3, k, l);
  cout << "l = 3*k: " << endl << "l: " << endl << l << endl;
  cout << "3*k: " << endl << mult<int64_t>(3, k) << endl;

  Matrix<int64_t> m(4, 0);
  m.rand(0, 10);
  cout << "m: " << endl << m << endl;*/

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

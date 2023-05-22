#include "../Fields/Mint.h"
#include <cassert>
#include <iostream>
#include <random>
using namespace std;

int main() {
    using T = int64_t;
    const T M = 998244353;
    Mint<T, M> m1(-100), m2(5);
    assert(m1 == M - 100);
    assert(pow(m2, (T) 1e18) * pow(m2, (T) -1e18) == 1);
    assert(m2 == 5);
    m1 *= m2;
    assert(m1 == M - 500);
    m1 += 512;
    assert(m1 == M + 12);
    uniform_int_distribution<T> gen(0, M - 1);
    mt19937 rng(777);
    for (int i = 0; i < 100; ++i) {
        Mint<T, M> x = gen(rng);
        if (x != 0) {
            auto y = invert(x);
            assert(y < x || y == x || y > x);
            assert(invert(y) == x);
            y.invert();
            assert(y == x);
        }
    }
    m1 = -m1;
    assert(m1 == M - 12);
    m1 = (1 * m1 * -1 + -12) / m1;
    assert(m1 == 0);
    m1.pow((T) 1 << 60);
    assert(m1 == 0);
    m1 += 2;
    m1.pow(30);
    assert(m1 == (T) 2147483648 / 2);
    m1 -= m1 *= m1;
    assert(m1 == 0);
    m2 /= m2;
    assert(m2 == 1);
    m2 = M - 1;
    m2 = m2 / m2;
    assert(m2 == 1);
    cout << "OK";
}

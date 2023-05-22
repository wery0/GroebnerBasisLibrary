#include "../Fields/Fraction.h"
#include <iostream>
using namespace std;

int main() {
    using T = int64_t;
    Fraction<T> f1(2, 10), f2(-6, -9);
    assert(f1 == Fraction<T>(1, 5));
    assert(f2 == Fraction<T>(2, 3));
    f1.invert();
    assert(f1 == Fraction<T>(5));
    f1 += f2;
    assert(f1 == Fraction<T>(17, 3));
    f2 += f2 += f2;
    assert(f2 == Fraction<T>(8, 3));
    f2.pow(0);
    assert(f2 == Fraction<T>(1));
    auto f3 = (f1 + f2) * f1 / (f1 - f2);
    assert(f3 == Fraction<T>(170, 21));
    f3 /= 10;
    assert(f3 == Fraction<T>(17, 21));
    f3 -= Fraction<T>(17, 21);
    assert(f3 == 0);
    assert(f3 < f1 && f3 <= f2 && f1 > f2 && f1 != f2);
    f3.pow((T) 1e18);
    assert(f3 == 0);
    f3 += Fraction<T>(1, 2);
    f3.pow(30);
    assert(f3 == Fraction<T>(1, 1 << 30));
    f3 -= f3 -= f3;
    assert(f3 == 0);
    f3 = -f2;
    assert(f3 == -1);
    f3 = f3 / Fraction<T>(4);
    assert(f3 * 4 == -1);
    auto f4 = 4 * f3 * -6;
    assert(f4 == 6);
    cout << "OK";
}

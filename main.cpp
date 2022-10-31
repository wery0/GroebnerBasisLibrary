#include "Fraction.h"
#include "Mint.h"
#include "Monomial.h"
#include "Polynomial.h"
#include "Variable.h"
#include <iostream>

using namespace std;

int main() {
    Fraction f1(1, 5);
    Fraction f2(1, 5);
    using mint = Mint<long long, 991>;
    mint x = 5, y = 57;
    x -= y;
    cout << x << endl;
    //Polynomial<Mint<int64_t, 997>> p("-x^1xxxxxxxyyxxyy + 35xy^3z + U_000321^0012");
    //Polynomial<Mint<int64_t, 997>> p("x + y + z");
    //Polynomial<Mint<int64_t, 997>, MonomialOrders::Lex> p("a + bc + cde + ade + a + eda");
    //Polynomial<Mint<int64_t, 997>, MonomialOrders::Grevlex> p("x^1y^5z^2 + x^4y^1z^3");
    Polynomial<Fraction<int64_t>> p("\\frac{-16}{-8}xxxyyyzzz - \\frac{1}{2}x^2y^3z^3U_83242348^123123 + 100h");
    cout << p << endl;
    cout << (5 * p * 6) * p << endl;
}

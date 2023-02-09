#include "Fraction.h"
#include "Ideal.h"
#include "Mint.h"
#include "Monomial.h"
#include "Polynomial.h"
#include "Variable.h"
#include <iostream>

using namespace std;

int main() {
    Fraction f1(1, 5);
    Fraction f2(1, 5);
    Fraction f3(1, 10);
    f3 += f2;
    f3.invert();
    invert(f3);
    cout << f3 << endl;
    using mint = Mint<long long, 991>;
    mint x = 5, y = 57;
    x -= y + 937;
    x = pow(x, 3);
    x.pow(2);
    cout << x << endl;
    Monomial<Fraction<int64_t>> m1("16xyz^45");
    Monomial<Fraction<int64_t>> m2("7xyz");
    cout << m1 / m2 << endl;
    Polynomial<Fraction<int64_t>, MonomialOrders::Lex> pp("abd + c");
    cout << pp << endl;
    //Polynomial<Mint<int64_t, 997>> p("-x^1xxxxxxxyyxxyy + 35xy^3z + U_000321^0012");
    //Polynomial<Mint<int64_t, 997>> p("x + y + z");
    //Polynomial<Mint<int64_t, 997>, MonomialOrders::Lex> p("a + bc + cde + ade + a + eda");
    //Polynomial<Mint<int64_t, 997>, MonomialOrders::Grevlex> p("x^1y^5z^2 + x^4y^1z^3");
    Polynomial<Fraction<int64_t>> p("\\frac{-16}{-8}xxxyyyzzz - \\frac{1}{2}x^2y^3z^3U_83242348^123123 + 100h");
    cout << endl;
    cout << p << endl;
    cout << (5 * p * 6) * p << endl;
    cout << endl;
    Polynomial<Mint<int64_t, 997>, MonomialOrders::Grlex, VariableOrders::InverseAsciiOrder> p1("x_1x_2x_3");
    Polynomial<Mint<int64_t, 997>, MonomialOrders::Grlex, VariableOrders::InverseAsciiOrder> p2("x_1x_3-x_4");
    p2.do_elementary_reduction_over(p1);
    cout << p1 << endl;
    cout << endl;

    Ideal<Polynomial<Mint<int64_t, 997>, MonomialOrders::Lex, VariableOrders::InverseAsciiOrder>> kek;
    kek.insert("x_1x_2-x_4");
    kek.insert("x_1x_3-x_4");
    kek.make_groebner_basis();
    kek.make_reduced_groebner_basis();
    kek.make_minimal_reduced_groebner_basis();
    kek.print();
    kek.clear();
    kek.insert("x^3 - 2xy");
    kek.insert("x^2y - 2y^2 + x");
    kek.insert("-x^2 + 6xy");
    kek.insert("-2xy");
    kek.insert("-2y^2 + x");
    kek.print();
    kek.make_groebner_basis();
    kek.print();
    kek.make_reduced_groebner_basis();
    kek.print();
    kek.make_minimal_reduced_groebner_basis();
    kek.print();
    kek.clear();
    cout << "VVVVVVV" << endl;
    kek.insert("x^5+y^4+z^3-1");
    kek.insert("x^3+y^3+z^2-1");
    kek.make_groebner_basis();
    kek.print();
}

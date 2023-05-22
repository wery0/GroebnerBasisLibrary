#include "../Library/Ideal.h"
#include <sstream>
using namespace std;

using F = Fraction<int64_t>;
using MF = Monomial<Fraction<int64_t>, VariableOrders::InverseAsciiOrder>;
using PMFL = Polynomial<MF, MonomialOrders::Lex>;
using PMFG = Polynomial<MF, MonomialOrders::Grlex>;
using PMFR = Polynomial<MF, MonomialOrders::Grevlex>;

namespace {
    /*
 * Reduced groebner basis of system {(x^{n+1} - yz^{n-1}w), (xy^{n-1} - z^n), (x^nz - y^nw)}
 * with grevlex ordering and x > y > z > w contains polynomial (z^{n^2+1}x - y^{n^2}w)
 */
    void Mora_test() {
        for (int n = 1; n <= 20; ++n) {
            Ideal<PMFR> ideal = {PMFR("a^" + to_string(n + 1) + " - bc^" + to_string(n - 1) + "d"),
                                 PMFR("ab^" + to_string(n - 1) + " - c^" + to_string(n)),
                                 PMFR("a^" + to_string(n) + "c - b^" + to_string(n) + "d")};
            ideal.make_reduced_groebner_basis();
            assert(ideal.basis_contains(PMFR("c^" + to_string(n * n + 1) + " - b^" + to_string(n * n) + "d")));
        }
    }
}// namespace

int main() {
    Mora_test();
    Ideal<PMFG> i1 = {"xz - y^2", "x^3 - z^2"};
    i1.make_groebner_basis();
    assert(i1.is_basis_equals_to({"xz - y^2", "x^3 - z^2", "x^2y^2 - z^3", "xy^4 - z^4", "y^6 - z^5"}));
    PMFG f1("-4x^2y^2z^2 + y^6 + 3z^5");
    PMFG f2("xy - 5z^2 + x");
    assert(i1.contains(f1));
    assert(i1.is_redusable_to_zero(f1));
    assert(!i1.contains(f2));
    assert(!i1.is_redusable_to_zero(f2));
    Ideal<PMFL> i2 = {"3a-6b-2c", "2a-4b+4d", "a-2b-c-d"};
    i2.make_minimal_groebner_basis();
    assert(i2.is_basis_equals_to({"a-2b-c-d", "c+3d"}));
    i2.make_reduced_groebner_basis();
    assert(i2.is_basis_equals_to({"a-2b+2d", "c+3d"}));
    i1.clear();
    i1.insert("x^3-2xy");
    i1.insert("x^2y-2y^2+x");
    i1.insert("-x^2");
    i1.insert("-2xy");
    i1.insert("-2y^2+x");
    i1.make_reduced_groebner_basis();
    assert(i1.is_basis_equals_to({"x^2", "xy", "y^2 - \\frac{1}{2}x"}));
    i1.clear();
    assert(i1.contains(PMFG("0")));
    assert(!i1.contains(PMFG("0+1")));
    cout << "OK";
}

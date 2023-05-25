#include "../Library/Polynomial.h"
#include <sstream>
using namespace std;

namespace {
    template<typename T>
    void check(const T& object, const std::string& must) {
        std::stringstream stream;
        stream << object;
        assert(stream.str() == must);
    }
}// namespace

int main() {
    using F = Fraction<int64_t>;
    using MF = Monomial<F, VariableOrders::InverseAsciiOrder>;
    using PMF = Polynomial<MF>;
    PMF p1("abc + bca");
    check(p1, "2abc");
    p1 += PMF("\\frac{1}{2}cba");
    check(p1, "\\frac{5}{2}abc");
    p1 /= F(5, 2);
    check(p1, "abc");
    p1 = p1 * MF("\\frac{990}{99}");
    check(p1, "10abc");
    p1 = p1 / MF("5");
    check(p1, "2abc");
    p1 *= p1 *= p1;
    check(p1, "16a^{4}b^{4}c^{4}");
    p1 = p1 / p1.get_highest_monomial();
    check(p1, "1");
    assert(p1.is_divisible_on_monomial(p1.get_highest_monomial()));
    p1 -= PMF("      1      - 001 + 1");
    check(p1, "0");
    assert(!p1.is_divisible_on_monomial(MF("0")));
    p1 = p1 + PMF("4x_0y_0^2z_0");
    check(p1, "4x_{0}y_{0}^{2}z_{0}");
    p1 /= p1.get_highest_monomial();
    check(p1, "1");
    p1 *= PMF("x + y") * PMF("x - y");
    check(p1, "x^{2} - y^{2}");
    p1 = p1 * p1;
    check(p1, "x^{4} - 2x^{2}y^{2} + y^{4}");
    assert(p1.get_degree() == 4);
    p1 -= p1.get_highest_monomial();
    assert(p1.is_divisible_on_monomial(MF("yy")));
    assert(!p1.is_divisible_on_monomial(MF("y^3")));
    assert(!p1.is_divisible_on_monomial(p1.get_highest_monomial()));
    p1 = p1 / MF("y^2");
    check(p1, "-2x^{2} + y^{2}");
    PMF p2("x^2y + xy^2"), p3("x + y");
    check(p2 * p3, "x^{3}y + 2x^{2}y^{2} + xy^{3}");
    PMF p4 = p2 * p3 / F(2, 1);
    check(p4, "\\frac{1}{2}x^{3}y + x^{2}y^{2} + \\frac{1}{2}xy^{3}");
    assert(p4 == -PMF("\\frac{128}{-256}x^3y - \\frac{-5}{-5}x^2y^2 + \\frac{-10}{20}xy^3"));
    assert(p4 != PMF("\\frac{1}{2}x^3y + \\frac{2}{3}x^2y^2 + \\frac{1}{2}xy^3"));
    p4 *= F(2);
    check(p4, "x^{3}y + 2x^{2}y^{2} + xy^{3}");
    assert(p4 == p2 * p3);
    p4 /= F(5);
    check(p4, "\\frac{1}{5}x^{3}y + \\frac{2}{5}x^{2}y^{2} + \\frac{1}{5}xy^{3}");
    p4 = p4 / F(1, 5);
    check(p4, "x^{3}y + 2x^{2}y^{2} + xy^{3}");
    p4 -= PMF("4yxxy");
    check(p4, "x^{3}y - 2x^{2}y^{2} + xy^{3}");
    p4 *= p4 *= p4 *= p4 *= p4;
    assert(p4 == PMF("x^48 y^16 - 32 x^47 y^17 + 496 x^46 y^18 - 4960 x^45 y^19 + 35960 x^44 y^20 - 201376 x^43 y^21 + "
                     "906192 x^42 y^22 - 3365856 x^41 y^23 + 10518300 x^40 y^24 - 28048800 x^39 y^25 + 64512240 x^38 "
                     "y^26 - 129024480 x^37 y^27 + 225792840 x^36 y^28 - 347373600 x^35 y^29 + 471435600 x^34 y^30 - "
                     "565722720 x^33 y^31 + 601080390 x^32 y^32 - 565722720 x^31 y^33 + 471435600 x^30 y^34 - "
                     "347373600 x^29 y^35 + 225792840 x^28 y^36 - 129024480 x^27 y^37 + 64512240 x^26 y^38 - 28048800 "
                     "x^25 y^39 + 10518300 x^24 y^40 - 3365856 x^23 y^41 + 906192 x^22 y^42 - 201376 x^21 y^43 + 35960 "
                     "x^20 y^44 - 4960 x^19 y^45 + 496 x^18 y^46 - 32 x^17 y^47 + x^16 y^48"));
    p4 -= p4;
    assert(p4 == PMF());
    check(p4, "0");
    p4.normalize();
    check(p4, "0");
    p4 = p1;
    check(p4, "-2x^{2} + y^{2}");
    p4.normalize();
    check(p4, "x^{2} - \\frac{1}{2}y^{2}");
    assert(get_S_polynomial(PMF("1"), PMF("1")) == PMF());
    assert(p4.get_highest_monomial_divisible_by(MF("yy")) == MF("\\frac{-1}{2}yy"));
    assert(p4.get_highest_monomial_divisible_by(MF("z")) == MF::ZeroMonomial());
    p1 = PMF("x_1x_2x_3");
    p2 = PMF("x_1x_3-x_4");
    assert(p2.do_one_elementary_reduction_over(p1));
    check(p1, "x_{2}x_{4}");
    cout << "OK";
}

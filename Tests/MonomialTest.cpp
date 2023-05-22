#include "../Library/Monomial.h"
using namespace std;

int main() {
    {
        using F = Fraction<int64_t>;
        using VI = Variable<int32_t, VariableOrders::InverseAsciiOrder>;
        using MF = Monomial<Fraction<int64_t>, VariableOrders::InverseAsciiOrder>;
        MF m1("16xyz^45"), m2("7xyz^1");
        assert(m1.is_divisible_on(m2));
        assert(m1 / m2 == MF(F(16, 7), VI('z'), 44));
        assert(m1 / m2 / MF("z^44") == MF("\\frac{16}{7}"));
        assert(m1.get_degree() == 47);
        m1 /= MF("z^45");
        assert(m1 == MF("16xy"));
        assert(m1.get_degree() == 2);
        assert(gcd(m1, MF("123321hkjsiwhdkK")) == MF("2") / MF("\\frac{-4}{-2}"));
        m1 *= MF("zzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzz");
        assert(gcd(m1, MF("zzz^41zz")) == MF("zzzzz^41"));
        assert(lcm(MF(), MF()) == MF("0") / MF("13245"));
        assert(lcm(MF(""), MF("")) == MF(""));
        MF m3("azAZ"), m4("a_0z_0Z_0A_0");
        assert(gcd(m3, m4) * lcm(m3, m4) == m3 * m4);
        assert(MF::ZeroMonomial() == MF("-000000000"));
        assert(MF("1").get_coefficient() == F(1));
        m3 /= m3;
        assert(m3 == MF("1"));
        m3 = 3 * m3 / 2;
        m3 *= MF("xyz");
        assert(m3 == MF("\\frac{3}{2}xyz"));
        m3 *= m3 *= m3;
        assert(m3 == MF("\\frac{81}{16}x^4y^4z^4"));
        m3 = m3 / m3;
        assert(m3 == MF("1"));
        assert(m3.get_degree() == 0);
        assert(m3.is_divisible_on(m3));
        m3.increase_coefficient(F(1, -1));
        assert(m3.is_zero());
        assert(!m3.is_divisible_on(m3));
        m3 = MF("\\frac{81}{16}x^4y^4z^4");
        m3.increase_coefficient(F(-81, 16));
        assert(m3.is_zero());
        assert(m3.get_degree() == 0);
    }
    {
        using M = Mint<int64_t, 7>;
        using MM = Monomial<M, VariableOrders::InverseAsciiOrder>;
        MM m1("-1a_234234"), m2 = -m1;
        assert(m1 == MM("6a_234234"));
        assert(m1.is_divisible_on(m2));
        assert(m2.is_divisible_on(m1));
        m1.increase_coefficient(1);
        assert(m1.is_zero());
        m1.increase_coefficient(6);
        assert(m1.get_coefficient() == (-m2).get_coefficient());
        m1 = MM("x^2y^2z^2"), m2 = MM("xzy^2");
        assert(gcd(m1, m2) * lcm(m1, m2) == m1 * m2);
        m1 /= m2;
        assert(m1 == MM("xz"));
    }
    cout << "OK";
}

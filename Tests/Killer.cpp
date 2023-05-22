#include "../Library/Ideal.h"

int main() {
    using monom = Monomial<Mint<int64_t, 2>, VariableOrders::InverseAsciiOrder>;
    Ideal<Polynomial<monom, MonomialOrders::Lex>> kek;
    kek.insert("x^5 + y^4 + z^3 - 1");
    kek.insert("x^3 + y^3 + z^2 - 1");
    kek.make_groebner_basis();
    std::cout << kek << '\n';
}

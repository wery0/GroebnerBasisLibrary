#include "../Library/Polynomial.h"
#include <sstream>

namespace {

    template<typename T>
    bool check(const T& object, const std::string& must) {
        std::stringstream stream;
        stream << object;
        return stream.str() == must;
    }

    void check_lex() {
        Polynomial<Monomial<Mint<int64_t, 997>, VariableOrders::AsciiOrder>, MonomialOrders::Lex> p1(
                "x + y + z + x^2 + y^2 + z^2 + xy + yz + zx + x^2y + xy^2 + x^2z + xz^2 + yz^2 + y^2z + xyz + x^2yz + "
                "xy^2z + xyz^2");
        Polynomial<Monomial<Mint<int64_t, 997>, VariableOrders::AsciiOrder>, MonomialOrders::Lex> p2(
                "x_1 + x_2 + x_3 + x_2x_1 + x_1x_2 + x_2^100 + x_1x_3^10 + y");
        Polynomial<Monomial<Fraction<int64_t>, VariableOrders::InverseAsciiOrder>, MonomialOrders::Lex> p3(
                "4xy^2z + 4z^2 - 5x^3 + 7x^2z^2");
        Polynomial<Monomial<Mint<int64_t, 997>, VariableOrders::AsciiOrder>, MonomialOrders::Lex> p4(
                "zy^5x^2 + z^4yx^3");

        std::string expected_p1 =
                "z^{2}yx + z^{2}y + z^{2}x + z^{2} + zy^{2}x + zy^{2} + zyx^{2} + zyx + zy + zx^{2} + zx + z + "
                "y^{2}x + y^{2} + yx^{2} + yx + y + x^{2} + x";
        std::cout << p1 << std::endl;
        assert(check(p1, expected_p1));


        std::string expected_p2 = "y + 2x_{1}x_{2} + x_{1}x_{3}^{10} + x_{1} + x_{2}^{100} + x_{2} + x_{3}";
        std::cout << p2 << std::endl;
        assert(check(p2, expected_p2));


        std::string expected_p3 = "-5x^{3} + 7x^{2}z^{2} + 4xy^{2}z + 4z^{2}";
        std::cout << p3 << std::endl;
        assert(check(p3, expected_p3));

        std::string expected_p4 = "z^{4}yx^{3} + zy^{5}x^{2}";
        std::cout << p4 << std::endl;
        assert(check(p4, expected_p4));
    }

    void check_grlex() {
        Polynomial<Monomial<Mint<int64_t, 997>, VariableOrders::AsciiOrder>, MonomialOrders::Grlex> p1(
                "x + y + z + x^2 + y^2 + z^2 + xy + yz + zx + x^2y + xy^2 + x^2z + xz^2 + yz^2 + y^2z + xyz + x^2yz + "
                "xy^2z + xyz^2");
        Polynomial<Monomial<Mint<int64_t, 997>, VariableOrders::AsciiOrder>, MonomialOrders::Grlex> p2(
                "x_1 + x_2 + x_3 + x_2x_1 + x_1x_2 + x_2^100 + x_1x_3^10 + y");
        Polynomial<Monomial<Fraction<int64_t>, VariableOrders::InverseAsciiOrder>, MonomialOrders::Grlex> p3(
                "4xy^2z + 4z^2 - 5x^3 + 7x^2z^2");
        Polynomial<Monomial<Mint<int64_t, 997>, VariableOrders::AsciiOrder>, MonomialOrders::Grlex> p4(
                "zy^5x^2 + z^4yx^3");

        Polynomial<Monomial<Mint<int64_t, 997>, VariableOrders::AsciiOrder>, MonomialOrders::Grlex> p5(
                "x_1 + x_2 + x_3 + y_1 + y_2 + y_3 + z_1 + z_2 + z_3");

        std::string expected_p1 =
                "z^{2}yx + zy^{2}x + zyx^{2} + z^{2}y + z^{2}x + zy^{2} + zyx + zx^{2} + y^{2}x + yx^{2} "
                "+ z^{2} + zy + zx + y^{2} + yx + x^{2} + z + y + x";
        std::cout << p1 << std::endl;
        assert(check(p1, expected_p1));

        std::string expected_p2 = "x_{2}^{100} + x_{1}x_{3}^{10} + 2x_{1}x_{2} + y + x_{1} + x_{2} + x_{3}";
        std::cout << p2 << std::endl;
        assert(check(p2, expected_p2));

        std::string expected_p3 = "7x^{2}z^{2} + 4xy^{2}z - 5x^{3} + 4z^{2}";
        std::cout << p3 << std::endl;
        assert(check(p3, expected_p3));

        std::string expected_p4 = "z^{4}yx^{3} + zy^{5}x^{2}";
        std::cout << p4 << std::endl;
        assert(check(p4, expected_p4));

        std::string expected_p5 = "z_{1} + z_{2} + z_{3} + y_{1} + y_{2} + y_{3} + x_{1} + x_{2} + x_{3}";
        std::cout << p5 << std::endl;
        assert(check(p5, expected_p5));
    }

    void check_grevlex() {
        Polynomial<Monomial<Mint<int64_t, 997>, VariableOrders::AsciiOrder>, MonomialOrders::Grevlex> p1(
                "x + y + z + x^2 + y^2 + z^2 + xy + yz + zx + x^2y + xy^2 + x^2z + xz^2 + yz^2 + y^2z + xyz + x^2yz + "
                "xy^2z + xyz^2");
        Polynomial<Monomial<Mint<int64_t, 997>, VariableOrders::AsciiOrder>, MonomialOrders::Grevlex> p2(
                "x_1 + x_2 + x_3 + x_2x_1 + x_1x_2 + x_2^100 + x_1x_3^10 + y");
        Polynomial<Monomial<Fraction<int64_t>, VariableOrders::InverseAsciiOrder>, MonomialOrders::Grevlex> p3(
                "4xy^2z + 4z^2 - 5x^3 + 7x^2z^2");
        Polynomial<Monomial<Mint<int64_t, 997>, VariableOrders::AsciiOrder>, MonomialOrders::Grevlex> p4(
                "zy^5x^2 + z^4yx^3");
        Polynomial<Monomial<Mint<int64_t, 997>, VariableOrders::AsciiOrder>, MonomialOrders::Grevlex> p5(
                "x_1 + x_2 + x_3 + y_1 + y_2 + y_3 + z_1 + z_2 + z_3");

        std::string expected_p1 =
                "z^{2}yx + zy^{2}x + zyx^{2} + z^{2}y + zy^{2} + z^{2}x + zyx + y^{2}x + zx^{2} + yx^{2} "
                "+ z^{2} + zy + y^{2} + zx + yx + x^{2} + z + y + x";
        std::cout << p1 << std::endl;
        assert(check(p1, expected_p1));

        std::string expected_p2 = "x_{2}^{100} + x_{1}x_{3}^{10} + 2x_{1}x_{2} + y + x_{1} + x_{2} + x_{3}";
        std::cout << p2 << std::endl;
        assert(check(p2, expected_p2));

        std::string expected_p3 = "4xy^{2}z + 7x^{2}z^{2} - 5x^{3} + 4z^{2}";
        std::cout << p3 << std::endl;
        assert(check(p3, expected_p3));

        std::string expected_p4 = "zy^{5}x^{2} + z^{4}yx^{3}";
        std::cout << p4 << std::endl;
        assert(check(p4, expected_p4));

        std::string expected_p5 = "z_{1} + z_{2} + z_{3} + y_{1} + y_{2} + y_{3} + x_{1} + x_{2} + x_{3}";
        std::cout << p5 << std::endl;
        assert(check(p5, expected_p5));
    }
}// namespace

int main() {
    std::cout << "Lex ordering:" << std::endl;
    check_lex();
    std::cout << std::endl << "Grlex ordering:" << std::endl;
    check_grlex();
    std::cout << std::endl << "Grevlex ordering:" << std::endl;
    check_grevlex();
    std::cout << "OK";
}

#pragma once
#include "../Fields/Fraction.h"
#include "../Fields/Mint.h"

namespace num_reader {
    template<typename U>
    size_t read_num(const std::string& s, U& var, size_t pos) {
        int32_t sgn = 1;
        if (s[pos] == '-' || s[pos] == '+') {
            if (s[pos] == '-') sgn = -1;
            ++pos;
        }
        assert(std::isdigit(s[pos]) && "Expected digit");
        for (; pos < s.size() && isdigit(s[pos]); ++pos) { var = var * 10 + s[pos] - '0'; }
        var *= sgn;
        return pos;
    }
}// namespace num_reader

template<typename CoefficientType>
struct CoefficientParser {
    CoefficientParser() { assert(0 && "This CoefficientType is not supported!"); }
};

template<typename T>
struct CoefficientParser<Fraction<T>> {
    Fraction<T> parse(std::string& s) {
        size_t l = 0;
        Fraction<T> coefficient = 1;
        int32_t coefficient_sign = 1;
        if (s[0] == '-' || s[0] == '+') {
            coefficient_sign = s[0] == '+' ? 1 : -1;
            ++l;
        }
        assert(l != s.size() && "Bad monomial");
        if (isdigit(s[l])) {
            coefficient = 0;
            l = num_reader::read_num(s, coefficient, l);
        } else if (s[l] == '\\') {
            assert(s.substr(l, 6) == "\\frac{" && "Expected fraction in form \\frac{numerator}{denominator}");
            int64_t numerator = 0, denominator = 0;
            size_t l1 = num_reader::read_num(s, numerator, l + 6);
            l = num_reader::read_num(s, denominator, l1 + 2) + 1;
            coefficient = Fraction(numerator, denominator);
        }
        s = s.substr(l);
        coefficient *= coefficient_sign;
        return coefficient;
    }
};

template<typename T, const T MOD>
struct CoefficientParser<Mint<T, MOD>> {
    Mint<T, MOD> parse(std::string& s) {
        size_t l = 0;
        Mint<T, MOD> coefficient = 1;
        coefficient = 1;
        int32_t coefficient_sign = 1;
        if (s[0] == '-' || s[0] == '+') {
            coefficient_sign = s[0] == '+' ? 1 : -1;
            ++l;
        }
        assert(l != s.size() && "Bad monomial");
        if (isdigit(s[l])) {
            coefficient = 0;
            l = num_reader::read_num(s, coefficient, l);
        }
        s = s.substr(l);
        coefficient *= coefficient_sign;
        return coefficient;
    }
};

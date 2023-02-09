#pragma once
#include "Fraction.h"
#include "Mint.h"
#include "MonomialOrders.h"
#include "Variable.h"
#include <iostream>
#include <map>

//Fraction should be passed in LaTeX style: (+|-)?\frac{numerator}{denominator}
//Fraction examples: -\frac{-12}{-12}, \frac{0}{1}
//Monomial should look like (Coefficient)?(Variable(^(\d)+)?)+
//Monomial examples: x, -234x_123^999yz^5, x^4y^3z^2
template<typename CoefficientType, typename DegreeType = int64_t, typename VariableNumberType = int32_t,
         typename VariableOrder = VariableOrders::AsciiOrder>
class Monomial {
    using Var = Variable<VariableNumberType, VariableOrder>;

public:
    Monomial() = default;

    explicit Monomial(const std::string& s) {
        MonomialParser parser;
        parser.parse(s, coefficient_, var_store_);
        simplify();
    }

    Monomial(CoefficientType coef, Var var, DegreeType deg) : coefficient_(coef) {
        assert(deg >= 0);
        if (coef != 0 && deg != 0) { var_store_[var] = deg; }
    }

    Monomial& operator*=(const Monomial& rhs) {
        coefficient_ *= rhs.coefficient_;
        for (const auto& [var, deg] : rhs.get_variables()) {
            if (!var_store_.count(var)) var_store_[var] = 0;
            var_store_[var] += deg;
        }
        return *this;
    }
    friend Monomial operator*(const Monomial& lhs, const Monomial& rhs) {
        Monomial res = lhs;
        res *= rhs;
        return res;
    }

    Monomial& operator*=(const CoefficientType& rhs) {
        coefficient_ *= rhs;
        if (coefficient_ == 0) { var_store_.clear(); }
        return *this;
    }
    friend Monomial operator*(const Monomial& lhs, const CoefficientType& rhs) {
        Monomial res = lhs;
        res *= rhs;
        return res;
    }
    friend Monomial operator*(const CoefficientType& lhs, const Monomial& rhs) {
        Monomial res = rhs;
        res *= lhs;
        return res;
    }

    Monomial& operator/=(const Monomial& rhs) {
        coefficient_ /= rhs.coefficient_;
        for (const auto& [var, deg] : rhs.var_store_) {
            var_store_[var] -= deg;
            assert(var_store_[var] >= 0 && "Variable power must be non-negative");
            if (var_store_[var] == 0) { var_store_.erase(var); }
        }
        return *this;
    }
    friend Monomial operator/(const Monomial& lhs, const Monomial& rhs) {
        Monomial res = lhs;
        res /= rhs;
        return res;
    }

    Monomial& operator/=(const CoefficientType& rhs) {
        assert(rhs != 0 && "Division by zero!");
        coefficient_ /= rhs;
        return *this;
    }
    friend Monomial operator/(const Monomial& lhs, const CoefficientType& rhs) {
        Monomial res = lhs;
        res /= rhs;
        return res;
    }

    Monomial operator-() const { return Monomial(-coefficient_, var_store_); }

    bool is_divisible_on(const Monomial& rhs) const {
        if (rhs.coefficient_ == 0) { return false; }
        for (const auto& [var, deg] : rhs.var_store_) {
            assert(deg > 0 && "Variable power must be positive");
            if (!var_store_.count(var) || var_store_.at(var) < deg) { return false; }
        }
        return true;
    }

    bool is_zero() const { return coefficient_ == 0; }

    CoefficientType get_coefficient() const { return coefficient_; }
    void increase_coefficient(const CoefficientType& coefficient) { coefficient_ += coefficient; }

    DegreeType get_degree() const {
        DegreeType ans = 0;
        for (const auto& [var, deg] : var_store_) { ans += deg; }
        return ans;
    }

    const std::map<Var, DegreeType>& get_variables() const { return var_store_; }

    friend Monomial gcd(const Monomial& m1, const Monomial& m2) {
        if (m1.is_zero() || m2.is_zero()) { return m1.is_zero() ? m2 : m1; }
        Monomial res("1");
        const auto& vars = m2.get_variables();
        for (const auto& [var, deg] : m1.get_variables()) {
            DegreeType cur = vars.count(var) ? vars.at(var) : 0;
            cur = std::min(cur, deg);
            res *= Monomial(1, var, cur);
        }
        return res;
    }

    friend Monomial lcm(const Monomial& m1, const Monomial& m2) {
        if (m1.is_zero() || m2.is_zero()) { return m1.is_zero() ? m2 : m1; }
        Monomial res = m1;
        res /= m1.get_coefficient();
        for (const auto& [var, deg] : m2.get_variables()) {
            const auto& vars = res.get_variables();
            DegreeType cur = vars.count(var) ? vars.at(var) : 0;
            if (cur < deg) { res *= Monomial(1, var, deg - cur); }
        }
        return res;
    }

    friend std::ostream& operator<<(std::ostream& os, const Monomial& monomial) {
        CoefficientType coef = monomial.coefficient_;
        assert(coef != 0);
        if (coef < 0) {
            os << "-";
            coef *= -1;
        }
        if (coef != 1 || monomial.var_store_.empty()) { os << coef; }
        for (auto it = monomial.var_store_.rbegin(); it != monomial.var_store_.rend(); ++it) {
            auto& [var, deg] = *it;
            assert(deg > 0);
            os << var;
            if (deg > 1) { os << "^{" << deg << "}"; }
        }
        return os;
    }

private:
    struct MonomialParser {
        void parse(const std::string& s, CoefficientType& coefficient, std::map<Var, DegreeType>& var_store) {
            if (s.empty()) { return; }
            size_t l = 0;
            coefficient = 1;
            int32_t coefficient_sign = 1;
            if (s[0] == '-' || s[0] == '+') {
                coefficient_sign = s[0] == '+' ? 1 : -1;
                ++l;
            }
            assert(l != s.size() && "Bad monomial");
            if (isdigit(s[l])) {
                coefficient = 0;
                l = read_num(s, coefficient, l);
            } else if (s[l] == '\\') {
                if constexpr (std::is_same_v<Fraction<>, CoefficientType>) {
                    assert(s.substr(l, 6) == "\\frac{" && "Expected fraction in form \\frac{numerator}{denominator}");
                    int64_t numerator = 0, denominator = 0;
                    size_t l1 = read_num(s, numerator, l + 6);
                    l = read_num(s, denominator, l1 + 2) + 1;
                    coefficient = Fraction(numerator, denominator);
                } else {
                    assert(0 && "Coefficient type should be Fraction");
                }
            }
            coefficient *= coefficient_sign;
            for (; l < s.size();) {
                size_t r = l;
                char var_name = s[l];
                VariableNumberType num = -1;
                if (r + 1 < s.size() && s[r + 1] == '_') {
                    num = 0;
                    r = read_num(s, num, r + 2) - 1;
                }
                DegreeType var_degree = 1;
                if (r + 1 < s.size() && s[r + 1] == '^') {
                    var_degree = 0;
                    r = read_num(s, var_degree, r + 2) - 1;
                }
                Var var{var_name, num};
                if (!var_store.count(var)) { var_store[var] = 0; }
                var_store[var] += var_degree;
                l = r + 1;
            }
        }

        template<typename T>
        size_t read_num(const std::string& s, T& var, size_t pos) const {
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
    };

    void simplify() {
        if (coefficient_ == 0) { var_store_.clear(); }
        for (auto it = var_store_.begin(); it != var_store_.end();) {
            if (it->second == 0) {
                it = var_store_.erase(it);
            } else {
                ++it;
            }
        }
    }

    Monomial(const CoefficientType& coefficient, const std::map<Var, DegreeType>& var_store)
        : coefficient_(coefficient), var_store_(var_store) {
        for (const auto& [var, deg] : var_store_) { assert(deg >= 0 && "Variable degree must be non-negative"); }
        simplify();
    }

    CoefficientType coefficient_ = 0;
    std::map<Var, DegreeType> var_store_;
};

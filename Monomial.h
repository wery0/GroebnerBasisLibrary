#pragma once
#include "Fraction.h"
#include "MonomialOrder.h"
#include "Variable.h"
#include <iostream>
#include <map>

//Fraction should be passed in LaTeX style: (+|-)?\frac{numerator}{denominator}
//Fraction examples: -\frac{-12}{-12}, \frac{0}{1}
//Monomial should look like (Coefficient)?(Variable(^(\d)+)?)+
//Monomial examples: x, -234x_123^999yz^5, x^4y^3z^2
template<typename CoefficientType, typename Order = MonomialOrders::Grlex, typename DegreeType = int64_t,
         typename VariableNumberType = int32_t>
class Monomial {
    template<typename X, typename Y, typename Z, typename T>
    friend class Polynomial;
    friend class MonomialOrders;

public:
    Monomial() = default;

    explicit Monomial(const std::string& s) {
        if (s.empty()) { return; }
        size_t l = 0;
        coefficient_ = 1;
        int32_t coefficient_sign = 1;
        if (s[0] == '-' || s[0] == '+') {
            coefficient_sign = s[0] == '+' ? 1 : -1;
            ++l;
        }
        if (l == s.size()) { throw std::runtime_error("Bad monomial: " + s); }
        if (isdigit(s[l])) {
            coefficient_ = 0;
            l = read_num(s, coefficient_, 0);
        } else if (s[l] == '\\') {
            if constexpr (std::is_same_v<Fraction<>, CoefficientType>) {
                if (s.substr(l, 6) != "\\frac{") {
                    throw std::runtime_error(
                            "Expected fraction in form \\frac{numerator}{denominator}, but got wrong prefix: " +
                            s.substr(l, 6));
                }
                int64_t numerator = 0, denominator = 0;
                size_t l1 = read_num(s, numerator, l + 6);
                l = read_num(s, denominator, l1 + 2) + 1;
                coefficient_ = Fraction(numerator, denominator);
            }
        }
        coefficient_ *= coefficient_sign;
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
            Variable<VariableNumberType> var{var_name, num};
            if (!var_store_.count(var)) { var_store_[var] = 0; }
            var_store_[var] += var_degree;
            l = r + 1;
        }
    }

    Monomial(const CoefficientType& coefficient, const std::map<Variable<VariableNumberType>, DegreeType>& var_store)
        : coefficient_(coefficient), var_store_(var_store) {}

    Monomial operator-() const { return Monomial(-coefficient_, var_store_); }

    Monomial operator*(const Monomial& rhs) const {
        Monomial res = Monomial(coefficient_ * rhs.coefficient_, var_store_);
        for (const auto& [var, deg] : rhs.var_store_) {
            if (!res.var_store_.count(var)) res.var_store_[var] = 0;
            res.var_store_[var] += deg;
        }
        return res;
    }

    Monomial operator/(const Monomial& rhs) const {
        assert(is_divisible_on(rhs));
        Monomial res = Monomial(coefficient_ / rhs.coefficient_, var_store_);
        for (const auto& [var, deg] : rhs.var_store_) {
            res.var_store_[var] -= deg;
            if (res.var_store_[var] == 0) { res.var_store_.erase(var); }
        }
        return res;
    }

    //doesn't consider coefficients
    bool operator==(const Monomial& rhs) const {
        if (var_store_.size() != rhs.var_store_.size()) { return false; }
        auto it1 = var_store_.begin();
        auto it2 = rhs.var_store_.begin();
        for (; it1 != var_store_.end(); ++it1, ++it2) {
            if ((*it1) != (*it2)) { return false; }
        }
        return true;
    }

    //doesn't consider coefficients
    bool operator!=(const Monomial& rhs) const { return !((*this) == rhs); }

    bool operator<(const Monomial& rhs) const {
        static Order order;
        return order(*this, rhs);
    }

    CoefficientType get_coefficient() const { return coefficient_; }

    bool empty() const { return var_store_.empty(); }

    bool is_divisible_on(const Monomial& rhs) const {
        if (rhs.coefficient_ == 0) { throw std::runtime_error("Division on zero monomial"); }
        for (const auto& [var, deg] : rhs.var_store_) {
            assert(deg > 0);
            if (!var_store_.count(var) || var_store_.at(var) < deg) { return false; }
        }
        return true;
    }

    DegreeType get_degree() const {
        DegreeType ans = 0;
        for (const auto& [var, deg] : var_store_) { ans += deg; }
        return ans;
    }
    friend DegreeType degree(const Monomial& monomial) { return monomial.get_degree(); }


    template<class ostream>
    friend ostream& operator<<(ostream& os, const Monomial& monomial) {
        CoefficientType coef = monomial.coefficient_;
        assert(coef != 0);
        if (coef < 0) {
            os << "-";
            coef *= -1;
        }
        if (coef != 1 || monomial.var_store_.empty()) { os << coef; }
        for (const auto& [var, deg] : monomial.var_store_) {
            assert(deg > 0);
            os << var;
            if (deg > 1) { os << "^{" << deg << "}"; }
        }
        return os;
    }

private:
    template<typename T>
    size_t read_num(const std::string& s, T& var, size_t pos) const {
        //std::cout<<s<<" "<<var<<" "<<pos<<std::endl;
        int32_t sgn = 1;
        if (s[pos] == '-' || s[pos] == '+') {
            if (s[pos] == '-') sgn = -1;
            ++pos;
        }
        if (!isdigit(s[pos])) { throw std::runtime_error("Expected digit, but got: " + s.substr(pos, 1)); }
        for (; pos < s.size() && isdigit(s[pos]); ++pos) { var = var * 10 + s[pos] - '0'; }
        var *= sgn;
        return pos;
    }

    mutable CoefficientType coefficient_;
    std::map<Variable<VariableNumberType>, DegreeType> var_store_;
};
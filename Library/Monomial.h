#pragma once
#include "../Orders/MonomialOrders.h"
#include "../Parsers/MonomialParser.h"
#include <iostream>

template<typename Iterator>
class Proxy {
public:
    Proxy(Iterator begin, Iterator end) : begin_(begin), end_(end) {}

    class ProxyIterator {
    public:
        ProxyIterator(Iterator iterator) : iterator_(iterator) {}

        ProxyIterator& operator++() {
            ++iterator_;
            return *this;
        }

        bool operator!=(const ProxyIterator& rhs) const { return iterator_ != rhs.iterator_; }
        bool operator==(const ProxyIterator& rhs) const { return iterator_ == rhs.iterator_; }

        auto operator*() { return *iterator_; }
        const auto operator*() const { return *iterator_; }

    private:
        Iterator iterator_;
    };

    ProxyIterator begin() const { return ProxyIterator(begin_); }
    ProxyIterator end() const { return ProxyIterator(end_); }

private:
    Iterator begin_;
    Iterator end_;
};

//Fraction should be passed in LaTeX style: (+|-)?\frac{numerator}{denominator}
//Fraction examples: -\frac{-12}{-12}, \frac{0}{1}
//Monomial should look like (Coefficient)?(Variable(^(\d)+)?)+
//Monomial examples: x, -234x_123^999yz^5, x^4y^3z^2
template<typename CoefficientType, typename VariableOrder = VariableOrders::AsciiOrder, typename DegreeType = int64_t,
         typename VariableNumberType = int32_t>
class Monomial {
    using Var = Variable<VariableNumberType, VariableOrder>;

public:
    using CoefficientType_ = CoefficientType;
    using DegreeType_ = DegreeType;

    Monomial() = default;

    explicit Monomial(std::string s) {
        MonomialParser<CoefficientType, VariableOrder, DegreeType, VariableNumberType> parser;
        parser.parse(s, &coefficient_, &var_store_);
        simplify();
    }

    Monomial(CoefficientType coefficient, Var var, DegreeType deg) : coefficient_(std::move(coefficient)) {
        assert(deg >= 0);
        if (coefficient != 0 && deg >= 0) { var_store_[var] = deg; }
    }

    Monomial& operator*=(const Monomial& rhs) {
        *this *= rhs.coefficient_;
        for (const auto& [var, deg] : rhs.var_store_) { var_store_[var] += deg; }
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
        assert(rhs.coefficient_ != 0);
        if (this == &rhs) {
            *this = Monomial("1");
            return *this;
        }
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
        if (coefficient_ == 0) { return true; }
        for (const auto& [var, deg] : rhs.var_store_) {
            assert(deg > 0 && "Variable power must be positive");
            if (!var_store_.count(var) || var_store_.at(var) < deg) { return false; }
        }
        return true;
    }

    bool is_zero() const { return coefficient_ == 0; }

    CoefficientType get_coefficient() const { return coefficient_; }
    void increase_coefficient(const CoefficientType& offset) {
        coefficient_ += offset;
        if (is_zero()) var_store_.clear();
    }

    DegreeType get_degree() const {
        DegreeType ans = 0;
        for (const auto& [var, deg] : var_store_) { ans += deg; }
        return ans;
    }

    Proxy<typename std::map<Var, DegreeType>::const_iterator> get_variables_ascending_order() const {
        return Proxy(var_store_.begin(), var_store_.end());
    }

    Proxy<typename std::map<Var, DegreeType>::const_reverse_iterator> get_variables_descending_order() const {
        return Proxy(var_store_.rbegin(), var_store_.rend());
    }

    static Monomial ZeroMonomial() { return Monomial(); }

    bool operator==(const Monomial& rhs) const {
        return coefficient_ == rhs.coefficient_ && var_store_ == rhs.var_store_;
    }
    friend bool operator!=(const Monomial& lhs, const Monomial& rhs) { return !(lhs == rhs); }


    friend Monomial gcd(const Monomial& m1, const Monomial& m2) {
        if (m1.is_zero() || m2.is_zero()) { return m1.is_zero() ? m2 : m1; }
        Monomial res("1");
        const auto& vars = m2.var_store_;
        for (const auto& [var, deg] : m1.var_store_) {
            DegreeType cur = vars.count(var) ? vars.at(var) : 0;
            cur = std::min(cur, deg);
            assert(cur >= 0);
            if (cur) res *= Monomial(1, var, cur);
        }
        return res;
    }

    friend Monomial lcm(const Monomial& m1, const Monomial& m2) {
        if (m1.is_zero() || m2.is_zero()) { return m1.is_zero() ? m2 : m1; }
        Monomial res = m1;
        res /= m1.get_coefficient();
        for (const auto& [var, deg] : m2.var_store_) {
            const auto& vars = res.var_store_;
            DegreeType cur = vars.count(var) ? vars.at(var) : 0;
            if (cur < deg) { res *= Monomial(1, var, deg - cur); }
        }
        return res;
    }

    friend std::ostream& operator<<(std::ostream& os, const Monomial& monomial) {
        if (monomial.is_zero()) { return os << "0"; }
        CoefficientType coef = monomial.coefficient_;
        if (coef < 0) {
            os << "-";
            coef *= -1;
        }
        if (coef != 1 || monomial.var_store_.empty()) { os << coef; }
        for (auto it = monomial.var_store_.rbegin(); it != monomial.var_store_.rend(); ++it) {
            const auto& [var, deg] = *it;
            assert(deg > 0);
            os << var;
            if (deg > 1) { os << "^{" << deg << "}"; }
        }
        return os;
    }

private:
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

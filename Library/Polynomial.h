#pragma once
#include "Monomial.h"
#include <algorithm>
#include <map>
#include <set>
#include <vector>

template<typename>
struct is_monomial : std::false_type {};

template<typename A, typename B, typename C, typename D>
struct is_monomial<Monomial<A, B, C, D>> : std::true_type {};

//Polynomial - sum of monomials
template<typename Monom, typename MonomialOrder = MonomialOrders::Grlex>
class Polynomial {
    static_assert(is_monomial<Monom>::value);
    using CoefficientType = typename Monom::CoefficientType_;
    using DegreeType = typename Monom::DegreeType_;

public:
    Polynomial() = default;

    Polynomial(const Monom& m) {
        if (!m.is_zero()) { monom_store_.insert(m); }
    }

    Polynomial(const Monom&& m) {
        if (!m.is_zero()) { monom_store_.insert(std::move(m)); }
    }

    Polynomial(std::string s) {
        s.erase(remove_if(s.begin(), s.end(), [](char c) { return std::isspace(c); }), s.end());
        for (size_t l = 0, r, dep; l < s.size();) {
            r = l, dep = s[l] == '{';
            while (r + 1 < s.size() && (dep > 0 || (dep == 0 && s[r + 1] != '-' && s[r + 1] != '+'))) {
                ++r;
                assert(!(dep == 0 && s[r] == '}') && "Bad parenthesis {} at input string");
                dep += s[r] == '{';
                dep -= s[r] == '}';
            }
            add(Monom(s.substr(l, r - l + 1)));
            l = r + 1;
        }
        remove_zero_monomials();
    }

    Polynomial& operator+=(const Polynomial& rhs) {
        for (const auto& monomial : rhs.monom_store_) { add(monomial); }
        remove_zero_monomials();
        return *this;
    }
    friend Polynomial operator+(const Polynomial& lhs, const Polynomial& rhs) {
        Polynomial res = lhs;
        res += rhs;
        return res;
    }

    Polynomial& operator-=(const Polynomial& rhs) {
        for (const auto& monomial : rhs.monom_store_) { add(-monomial); }
        remove_zero_monomials();
        return *this;
    }
    friend Polynomial operator-(const Polynomial& lhs, const Polynomial& rhs) {
        Polynomial res = lhs;
        res -= rhs;
        return res;
    }
    Polynomial operator-() const { return *this * CoefficientType(-1); }

    Polynomial& operator*=(const CoefficientType& rhs) {
        if (rhs == 0) {
            monom_store_.clear();
        } else {
            std::set<Monom, MonomialOrder> new_store;
            for (const auto& monomial : monom_store_) { new_store.insert(monomial * rhs); }
            monom_store_ = std::move(new_store);
        }
        return *this;
    }
    friend Polynomial operator*(const Polynomial& lhs, const CoefficientType& rhs) {
        Polynomial res = lhs;
        res *= rhs;
        return res;
    }
    friend Polynomial operator*(const CoefficientType& lhs, const Polynomial& rhs) {
        Polynomial res = rhs;
        res *= lhs;
        return res;
    }

    Polynomial& operator*=(const Monom& rhs) {
        std::set<Monom, MonomialOrder> new_store;
        for (const auto& monom1 : monom_store_) { new_store.insert(monom1 * rhs); }
        monom_store_ = std::move(new_store);
        remove_zero_monomials();
        return *this;
    }
    friend Polynomial operator*(const Polynomial& lhs, const Monom& rhs) {
        Polynomial res = lhs;
        res *= rhs;
        return res;
    }
    friend Polynomial operator*(const Monom& lhs, const Polynomial& rhs) {
        Polynomial res = rhs;
        res *= lhs;
        return res;
    }

    Polynomial& operator*=(const Polynomial& rhs) {
        Polynomial<Monom, MonomialOrder> res;
        for (const auto& monom1 : monom_store_) {
            for (const auto& monom2 : rhs.monom_store_) { res.add(monom1 * monom2); }
        }
        return *this = std::move(res);
    }
    friend Polynomial operator*(const Polynomial& lhs, const Polynomial& rhs) {
        Polynomial res = lhs;
        res *= rhs;
        return res;
    }

    Polynomial& operator/=(const CoefficientType& rhs) { return (*this) *= invert(rhs); }
    friend Polynomial operator/(const Polynomial& lhs, const CoefficientType& rhs) {
        Polynomial res = lhs;
        res /= rhs;
        return res;
    }

    Polynomial& operator/=(const Monom& rhs) {
        std::set<Monom, MonomialOrder> new_store;
        for (const auto& monom : monom_store_) { new_store.insert(monom / rhs); }
        monom_store_ = std::move(new_store);
        return *this;
    }
    friend Polynomial operator/(const Polynomial& lhs, const Monom& rhs) {
        Polynomial res = lhs;
        res /= rhs;
        return res;
    }

    bool operator==(const Polynomial& rhs) const { return monom_store_ == rhs.monom_store_; }
    friend bool operator!=(const Polynomial& lhs, const Polynomial& rhs) { return !(lhs == rhs); }

    bool is_divisible_on_monomial(const Monom& rhs) {
        if (rhs.is_zero()) return false;
        for (const auto& monomial : monom_store_) {
            if (!monomial.is_divisible_on(rhs)) { return false; }
        }
        return true;
    }

    bool is_zero() const { return monom_store_.empty(); }

    DegreeType get_degree() const {
        DegreeType ans = 0;
        for (const auto& monomial : monom_store_) { ans = std::max(ans, monomial.get_degree()); }
        return ans;
    }

    Monom get_highest_monomial() const { return is_zero() ? Monom::ZeroMonomial() : *monom_store_.rbegin(); }

    Monom get_highest_monomial_divisible_by(const Monom& m) const {
        for (auto it = monom_store_.rbegin(); it != monom_store_.rend(); ++it) {
            if (it->is_divisible_on(m)) { return *it; }
        }
        return Monom::ZeroMonomial();
    }

    bool do_one_elementary_reduction_over(Polynomial& p) const {
        Monom monom = p.get_highest_monomial_divisible_by(get_highest_monomial());
        if (monom.is_zero()) { return false; }
        p -= monom / get_highest_monomial() * (*this);
        return true;
    }

    void normalize() {
        if (!is_zero()) { (*this) /= get_highest_monomial().get_coefficient(); }
    }

    friend Polynomial get_S_polynomial(const Polynomial& p1, const Polynomial& p2) {
        assert(!p1.is_zero() && !p2.is_zero());
        auto lc = lcm(p1.get_highest_monomial(), p2.get_highest_monomial());
        auto m1 = lc / p1.get_highest_monomial();
        auto m2 = lc / p2.get_highest_monomial();
        return p1 * m1 - p2 * m2;
    }

    friend std::ostream& operator<<(std::ostream& os, const Polynomial& polynomial) {
        if (polynomial.is_zero()) { return os << "0"; }
        for (auto it = polynomial.monom_store_.rbegin(); it != polynomial.monom_store_.rend(); ++it) {
            const Monom& monomial = *it;
            assert(monomial.get_coefficient() != 0);
            if (it == polynomial.monom_store_.rbegin()) {
                os << monomial;
            } else {
                os << " ";
                if (monomial.get_coefficient() > 0) {
                    os << "+ " << monomial;
                } else {
                    os << "- " << (-monomial);
                }
            }
        }
        return os;
    }

private:
    void add(const Monom& m) {
        if (!monom_store_.count(m)) {
            if (!m.is_zero()) monom_store_.insert(m);
        } else {
            auto nh = monom_store_.extract(m);
            nh.value().increase_coefficient(m.get_coefficient());
            if (!nh.value().is_zero()) monom_store_.insert(std::move(nh));
        }
    }

    void remove_zero_monomials() {
        for (auto it = monom_store_.begin(); it != monom_store_.end();) {
            if (it->get_coefficient() == 0) {
                it = monom_store_.erase(it);
            } else {
                ++it;
            }
        }
    }

    std::set<Monom, MonomialOrder> monom_store_;
};

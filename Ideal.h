#pragma once
#include "Polynomial.h"

template<typename>
struct is_polynomial : std::false_type {};

template<typename A, typename B, typename C, typename D, typename E>
struct is_polynomial<Polynomial<A, B, C, D, E>> : std::true_type {};

enum BasisType { Any, Groebner, MinimalGroebner, MinimalReducedGroebner };

template<typename Polynom>
class Ideal {
    static_assert(is_polynomial<Polynom>::value);

public:
    Ideal() = default;

    Ideal(const std::vector<Polynom>& store) {
        for (auto& p : store) { insert(p); }
    }

    void insert(const Polynom& p) {
        if (!p.is_zero()) {
            basis_type_ = BasisType::Any;
            store_.push_back(p);
            store_.back().normalize();
        }
    }

    void insert(const std::string& s) { insert(Polynom(s)); }

    void reduce(Polynom& rhs) const {
        for (;;) {
            bool was_reduced = false;
            for (const Polynom& p : store_) { was_reduced |= p.do_elementary_reduction_over(rhs); }
            if (!was_reduced) { break; }
        }
    }

    bool is_redusable_to_zero(Polynom rhs) const {
        reduce(rhs);
        return rhs.is_zero();
    }

    void make_groebner_basis() {
        if (basis_type_ != BasisType::Any) { return; }
        for (int i = 0; i < store_.size(); ++i) {
            for (int j = 0; j < i; ++j) {
                if (gcd(store_[i].get_highest_monomial(), store_[j].get_highest_monomial()).get_variables().empty()) {
                    continue;
                }
                Polynom p = get_S_polynomial(store_[i], store_[j]);
                reduce(p);
                insert(p);
            }
        }
        basis_type_ = BasisType::Groebner;
    }

    void make_minimal_groebner_basis() {
        if (basis_type_ == BasisType::MinimalGroebner || basis_type_ == BasisType::MinimalReducedGroebner) { return; }
        make_groebner_basis();
        for (int q = 0; q < store_.size(); q++) {
            for (int w = 0; w < store_.size(); w++) {
                if (q == w) continue;
                if (store_[q].get_highest_monomial().is_divisible_on(store_[w].get_highest_monomial())) {
                    store_.erase(store_.begin() + q);
                    q--;
                    break;
                }
            }
        }
        basis_type_ = BasisType::MinimalGroebner;
    }

    void make_minimal_reduced_groebner_basis() {
        if (basis_type_ == BasisType::MinimalReducedGroebner) { return; }
        make_minimal_groebner_basis();
        for (int q = 0; q < store_.size(); q++) {
            std::vector<Polynom> other = store_;
            other.erase(other.begin() + q);
            Ideal<Polynom> i(other);
            Polynom tmp = store_[q];
            i.reduce(tmp);
            store_[q] = tmp;
        }
        for (const auto& p : store_) { assert(p.get_highest_monomial().get_coefficient() == 1); }
        basis_type_ = BasisType::MinimalReducedGroebner;
    }

    bool contains(Polynom p) {
        make_groebner_basis();
        return is_redusable_to_zero(p);
    }

    void clear() {
        basis_type_ = BasisType::Any;
        store_.clear();
    }

    void print() {
        std::cout << "{\n";
        for (const auto& p : store_) { std::cout << '\t' << p << '\n'; }
        std::cout << "}\n";
    }

private:
    Polynom get_S_polynomial(const Polynom& p1, const Polynom& p2) {
        auto lc = lcm(p1.get_highest_monomial(), p2.get_highest_monomial());
        auto m1 = lc / p1.get_highest_monomial();
        auto m2 = lc / p2.get_highest_monomial();
        return p1 * m1 - p2 * m2;
    }

    std::vector<Polynom> store_;
    BasisType basis_type_ = BasisType::Any;
};

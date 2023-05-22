#pragma once
#include "Polynomial.h"

template<typename>
struct is_polynomial : std::false_type {};

template<typename A, typename B>
struct is_polynomial<Polynomial<A, B>> : std::true_type {};

enum BasisType { Any, Groebner, MinimalGroebner, ReducedGroebner };

template<typename Polynom>
class Ideal {
    static_assert(is_polynomial<Polynom>::value);

public:
    Ideal() = default;

    Ideal(const std::initializer_list<Polynom>& list) {
        for (const auto& p : list) { insert(p); }
    }

    Ideal(const std::initializer_list<std::string>& list) {
        for (const auto& s : list) { insert(s); }
    }

    Ideal(const std::vector<Polynom>& store) {
        for (const auto& p : store) { insert(p); }
    }

    Ideal(const std::vector<Polynom>&& store) {
        for (const auto& p : store) { insert(std::move(p)); }
    }

    void insert(const Polynom& p) {
        if (!p.is_zero()) {
            basis_type_ = BasisType::Any;
            store_.push_back(p);
            store_.back().normalize();
        }
    }

    void insert(const Polynom&& p) {
        if (!p.is_zero()) {
            basis_type_ = BasisType::Any;
            store_.push_back(std::move(p));
            store_.back().normalize();
        }
    }

    void insert(const std::string& s) { insert(Polynom(s)); }

    bool reduce_by_set_once(Polynom* rhs) const {
        bool was_reduced = false;
        for (const Polynom& p : store_) { was_reduced |= p.do_one_elementary_reduction_over(*rhs); }
        return was_reduced;
    }

    void reduce(Polynom* rhs) const {
        while (reduce_by_set_once(rhs)) {}
    }

    bool is_redusable_to_zero(Polynom rhs) const {
        reduce(&rhs);
        return rhs.is_zero();
    }

    void make_groebner_basis() {
        if (basis_type_ != BasisType::Any) { return; }
        for (size_t i = 0; i < store_.size(); ++i) {
            for (size_t j = 0; j < i; ++j) {
                if (are_leading_monomials_coprime(store_[i], store_[j])) { continue; }
                Polynom p = get_S_polynomial(store_[i], store_[j]);
                reduce(&p);
                insert(std::move(p));
            }
        }
        basis_type_ = BasisType::Groebner;
    }

    void make_minimal_groebner_basis() {
        if (basis_type_ == BasisType::MinimalGroebner || basis_type_ == BasisType::ReducedGroebner) { return; }
        make_groebner_basis();
        exclude_unnecessary_polinomials();
        basis_type_ = BasisType::MinimalGroebner;
    }

    void make_reduced_groebner_basis() {
        if (basis_type_ == BasisType::ReducedGroebner) { return; }
        make_minimal_groebner_basis();
        reduce_each();
        assert(are_all_polynomials_normalized());
        basis_type_ = BasisType::ReducedGroebner;
    }

    bool contains(Polynom p) {
        make_groebner_basis();
        return is_redusable_to_zero(p);
    }

    bool basis_contains(const Polynom& p) const {
        for (const auto& polynom : store_) {
            if (p == polynom) { return true; }
        }
        return false;
    }

    bool is_basis_equals_to(const std::initializer_list<std::string>& list) const {
        if (list.size() != store_.size()) { return false; }
        for (const auto& s : list) {
            if (!basis_contains(s)) { return false; }
        }
        return true;
    }

    void clear() {
        basis_type_ = BasisType::Any;
        store_.clear();
    }

    friend std::ostream& operator<<(std::ostream& os, const Ideal& ideal) {
        os << "{\n";
        for (const auto& p : ideal.store_) { os << '\t' << p << '\n'; }
        return os << "}\n";
    }

private:
    bool are_all_polynomials_normalized() const {
        for (const auto& p : store_) {
            if (p.get_highest_monomial().get_coefficient() != 1) return false;
        }
        return true;
    }

    void exclude_unnecessary_polinomials() {
        for (size_t i = 0; i < store_.size(); ++i) {
            for (size_t j = 0; j < store_.size(); ++j) {
                if (i == j) continue;
                if (are_leading_monomials_divisible(store_[i], store_[j])) {
                    if (i + 1 != store_.size()) { std::swap(store_[i], store_.back()); }
                    store_.pop_back();
                    --i;
                    break;
                }
            }
        }
    }

    void reduce_each() {
        for (size_t i = 0; i < store_.size(); ++i) {
            Polynom tmp = store_[i];
            store_.erase(store_.begin() + i);
            Ideal<Polynom> ideal(store_);
            ideal.reduce(&tmp);
            store_.insert(store_.begin() + i, tmp);
        }
    }

    static bool are_leading_monomials_coprime(const Polynom& p1, const Polynom& p2) {
        return gcd(p1.get_highest_monomial(), p2.get_highest_monomial()).is_zero();
    }

    static bool are_leading_monomials_divisible(const Polynom& p1, const Polynom& p2) {
        return p1.get_highest_monomial().is_divisible_on(p2.get_highest_monomial());
    }

    std::vector<Polynom> store_;
    BasisType basis_type_ = BasisType::Any;
};

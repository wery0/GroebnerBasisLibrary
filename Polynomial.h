#include "Monomial.h"
#include <algorithm>
#include <set>
#include <unordered_map>
#include <vector>

//Polynomial - sum of monomials
template<typename CoefficientType, typename Order = MonomialOrders::Grlex, typename DegreeType = int64_t,
         typename VariableNumberType = int32_t>
class Polynomial {
    using Monom = Monomial<CoefficientType, Order, DegreeType, VariableNumberType>;

public:
    Polynomial() = default;

    Polynomial(Monom m) {
        if (m.coefficient_ != 0) { monom_store_.insert(m); }
    }

    Polynomial(std::string s) {
        s.erase(remove(s.begin(), s.end(), ' '), s.end());
        s.erase(remove(s.begin(), s.end(), '\n'), s.end());
        for (size_t l = 0, r, dep; l < s.size();) {
            r = l, dep = s[l] == '{';
            while (r + 1 < s.size() && (dep > 0 || (dep == 0 && s[r + 1] != '-' && s[r + 1] != '+'))) {
                ++r;
                if (dep == 0 && s[r] == '}') { throw std::runtime_error("Bad parenthesis {} at input string: " + s); }
                dep += s[r] == '{';
                dep -= s[r] == '}';
            }
            add(Monom(s.substr(l, r - l + 1)));
            l = r + 1;
        }
        RemoveZeroMonomials();
    }

    Polynomial operator+(const Polynomial& rhs) const {
        Polynomial res(monom_store_);
        for (const auto& monomial : rhs.monom_store_) { res.add(monomial); }
        res.RemoveZeroMonomials();
        return res;
    }

    void operator+=(const Polynomial& rhs) {
        for (const auto& monomial : rhs.monom_store_) { add(monomial); }
        RemoveZeroMonomials();
    }

    Polynomial operator-(const Polynomial& rhs) const {
        Polynomial res(monom_store_);
        for (const auto& monomial : rhs.monom_store_) { res.add(-monomial); }
        res.RemoveZeroMonomials();
        return res;
    }

    void operator-=(const Polynomial& rhs) {
        for (const auto& monomial : rhs.monom_store_) { add(-monomial); }
        RemoveZeroMonomials();
    }

    Polynomial operator*(const CoefficientType& rhs) const {
        if (rhs == 0) { return {}; }
        Polynomial res(monom_store_);
        for (const auto& monomial : res.monom_store_) { monomial.coefficient_ *= rhs; }
        return res;
    }
    friend Polynomial operator*(const CoefficientType& num, const Polynomial& poly) { return poly * num; }

    Polynomial operator*(const Monom& rhs) const {
        Polynomial res;
        for (const auto& monomial : monom_store_) { res.add(monomial * rhs); }
        res.RemoveZeroMonomials();
        return res;
    }
    friend Polynomial operator*(const Monom& monom, const Polynomial& poly) { return poly * monom; }

    Polynomial operator*(const Polynomial& rhs) const {
        Polynomial res;
        for (const auto& monom1 : monom_store_) {
            for (const auto& monom2 : rhs.monom_store_) { res.add(monom1 * monom2); }
        }
        res.RemoveZeroMonomials();
        return res;
    }

    Polynomial operator/(const CoefficientType& rhs) const { return (*this) * inv(rhs); }

    bool operator==(const Polynomial& rhs) const {
        if (monom_store_.size() != rhs.monom_store_.size()) { return false; }
        auto it1 = monom_store_.begin();
        auto it2 = rhs.monom_store_.begin();
        for (; it1 != monom_store_.end(); ++it1, ++it2) {
            if ((*it1) != (*it2)) { return false; }
        }
        return true;
    }

    bool operator!=(const Polynomial& rhs) const { return !((*this) == rhs); }

    bool empty() const { return monom_store_.empty(); }

    //Unstated variables have default value 1, i. e. not affect the result.
    CoefficientType eval(std::unordered_map<Variable<VariableNumberType>, CoefficientType> variable_values) const {
        CoefficientType res = 0;
        for (const auto& monomial : monom_store_) {
            CoefficientType cur = monomial.koef.num;
            for (const auto& [var, deg] : monomial.var_store_) {
                if (!variable_values.count(var)) { continue; }
                CoefficientType tyt = binpow(variable_values[var.c], deg);
                cur = cur * tyt;
            }
            res += cur;
        }
        return res;
    }

    DegreeType get_degree() const {
        DegreeType ans = 0;
        for (const auto& monomial : monom_store_) { ans = max(ans, degree(monomial)); }
        return ans;
    }
    friend DegreeType degree(const Polynomial& polynomial) { return polynomial.get_degree(); }

    template<class ostream>
    friend ostream& operator<<(ostream& os, const Polynomial& polynomial) {
        if (polynomial.empty()) os << "0";
        for (auto it = polynomial.monom_store_.begin(); it != polynomial.monom_store_.end(); ++it) {
            const Monom& monomial = *it;
            assert(monomial.get_coefficient() != 0);
            if (it == polynomial.monom_store_.begin()) {
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
    explicit Polynomial(const std::set<Monom>& store) : monom_store_(store) {}

    void add(const Monom& m) {
        if (!monom_store_.count(m)) {
            monom_store_.insert(m);
        } else {
            (*monom_store_.find(m)).coefficient_ += m.coefficient_;
        }
        if ((*monom_store_.find(m)).coefficient_ == 0) { monom_store_.erase(m); }
    }

    void RemoveZeroMonomials() {
        std::vector<Monom> zero_monoms;
        for (const auto& monomial : monom_store_) {
            if (monomial.coefficient_ == 0) { zero_monoms.push_back(monomial); }
        }
        for (const auto& monomial : zero_monoms) { monom_store_.erase(monomial); }
    }

    std::set<Monom> monom_store_;
};
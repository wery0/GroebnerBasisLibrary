#pragma once
#include <algorithm>
#include <iostream>

namespace MonomialOrders {
    struct Lex {
        template<typename T>
        bool operator()(const T& m1, const T& m2) const {
            const auto& v1 = m1.get_variables();
            const auto& v2 = m2.get_variables();
            return std::lexicographical_compare(v1.rbegin(), v1.rend(), v2.rbegin(), v2.rend());
        }
    };

    struct Grlex {
        template<typename T>
        bool operator()(const T& m1, const T& m2) const {
            auto deg1 = m1.get_degree();
            auto deg2 = m2.get_degree();
            if (deg1 != deg2) { return deg1 < deg2; }
            static Lex l;
            return l(m1, m2);
        }
    };

    struct Grevlex {
        template<typename T>
        bool operator()(const T& m1, const T& m2) const {
            auto deg1 = m1.get_degree();
            auto deg2 = m2.get_degree();
            if (deg1 != deg2) { return deg1 < deg2; }
            const auto& v1 = m1.get_variables();
            const auto& v2 = m2.get_variables();
            if (v1 == v2) { return false; }
            auto it1 = v1.begin();
            auto it2 = v2.begin();
            for (; it1 != v1.end() && it2 != v2.end(); ++it1, ++it2) {
                auto [var1, d1] = *it1;
                auto [var2, d2] = *it2;
                if (var1 != var2) return var1 < var2;
                if (d1 != d2) return d1 > d2;
            }
            return v1.size() < v2.size();
        }
    };
}// namespace MonomialOrders

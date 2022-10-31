#pragma once
#include <algorithm>

struct MonomialOrders{
    struct Lex {
        template<typename T>
        bool operator()(const T& m1, const T& m2) const {
            return std::lexicographical_compare(m2.var_store_.begin(), m2.var_store_.end(),
                                                m1.var_store_.begin(), m1.var_store_.end());
        }
    };

    struct Grlex {
        template<typename T>
        bool operator()(const T& m1, const T& m2) const {
            auto deg1 = m1.get_degree();
            auto deg2 = m2.get_degree();
            if (deg1 != deg2) { return deg1 > deg2; }
            return std::lexicographical_compare(m2.var_store_.begin(), m2.var_store_.end(),
                                                m1.var_store_.begin(), m1.var_store_.end());
        }
    };

    struct Grevlex {
        template<typename T>
        bool operator()(const T& m1, const T& m2) const {
            auto deg1 = m1.get_degree();
            auto deg2 = m2.get_degree();
            if (deg1 != deg2) { return deg1 > deg2; }
            return std::lexicographical_compare(m1.var_store_.rbegin(), m1.var_store_.rend(),
                                                m2.var_store_.rbegin(), m2.var_store_.rend());
        }
    };
};
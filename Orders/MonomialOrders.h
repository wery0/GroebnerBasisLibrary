#pragma once
#include <algorithm>
#include <iostream>

namespace MonomialOrders {

    template<class InputIt1, class InputIt2, class Compare>
    bool lexicographical_compare(InputIt1 first1, InputIt1 last1, InputIt2 first2, InputIt2 last2, Compare comp) {
        for (; (first1 != last1) && (first2 != last2); ++first1, ++first2) {
            if (comp(*first1, *first2)) return true;
            if (comp(*first2, *first1)) return false;
        }
        return (first1 == last1) && (first2 != last2);
    }

    template<class InputIt1, class InputIt2>
    bool lexicographical_compare(InputIt1 first1, InputIt1 last1, InputIt2 first2, InputIt2 last2) {
        for (; (first1 != last1) && (first2 != last2); ++first1, ++first2) {
            if (*first1 < *first2) return true;
            if (*first2 < *first1) return false;
        }
        return (first1 == last1) && (first2 != last2);
    }

    struct Lex {
        template<typename T>
        bool operator()(const T& m1, const T& m2) const {
            auto v1 = m1.get_variables_descending_order();
            auto v2 = m2.get_variables_descending_order();
            return lexicographical_compare(v1.begin(), v1.end(), v2.begin(), v2.end());
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
            auto v1 = m1.get_variables_ascending_order();
            auto v2 = m2.get_variables_ascending_order();
            auto cmp = [](auto p1, auto p2) {
                return p1.first != p2.first ? p1.first < p2.first : p1.second != p2.second ? p1.second > p2.second : 0;
            };
            return lexicographical_compare(v1.begin(), v1.end(), v2.begin(), v2.end(), cmp);
        }
    };
}// namespace MonomialOrders

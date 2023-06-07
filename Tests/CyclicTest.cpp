#include "../Library/Ideal.h"
#include <chrono>

using namespace std;

using M = Mint<int64_t, 998244353>;
using MM = Monomial<M, VariableOrders::InverseAsciiOrder>;
using PMFR = Polynomial<MM, MonomialOrders::Grevlex>;

void test_cyclic(int n) {
    auto start_time = chrono::steady_clock::now();
    Ideal<PMFR> ideal;
    for (int i = 1; i <= n; ++i) {
        PMFR polynomial;
        if (i < n) {
            for (int j = 0; j < n; ++j) {
                MM monomial("1");
                for (int k = j; k < j + i; ++k) {
                    Variable<int32_t, VariableOrders::InverseAsciiOrder> var('x', k % n);
                    monomial *= var;
                }
                polynomial += monomial;
            }
        } else {
            MM monomial("1");
            for (int k = 0; k < n; ++k) {
                Variable<int32_t, VariableOrders::InverseAsciiOrder> var('x', k);
                monomial *= var;
            }
            polynomial += monomial;
            polynomial -= PMFR("1");
        }
        ideal.insert(polynomial);
    }
    ideal.make_reduced_groebner_basis();
    auto finish_time = chrono::steady_clock::now();
    auto work_time = chrono::duration_cast<chrono::microseconds>(finish_time - start_time).count();
    cout << "N = " << n << endl;
    cout << "Ideal size: " << ideal.size() << endl;
    cout << "Time: " << work_time / 1e6 << " s" << endl;
}

int main() {
    for (int n = 1; n <= 6; ++n) { test_cyclic(n); }
}

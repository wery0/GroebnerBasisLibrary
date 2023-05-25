#include "../Library/Ideal.h"
#include <chrono>

using namespace std;

using M = Mint<int64_t, 998244353>;
using MM = Monomial<M, VariableOrders::InverseAsciiOrder>;
using PMFR = Polynomial<MM, MonomialOrders::Grevlex>;

void test_katsura(int n) {
    auto start_time = chrono::steady_clock::now();
    Ideal<PMFR> ideal;
    {
        PMFR polynomial("x_0-1");
        for (int i = 1; i < n; ++i) polynomial += MM("2x_" + to_string(i));
        ideal.insert(polynomial);
    }
    {
        PMFR polynomial("x_0^2-x_0");
        for (int i = 1; i < n; ++i) polynomial += MM("2x_" + to_string(i) + "^2");
        ideal.insert(polynomial);
    }
    for (int i = 0; i < (n + (n & 1)) / 2 - 1; ++i) {
        PMFR poly("-x_" + to_string(2 * i + 1));
        for (int j = 1; j <= i; ++j) poly += MM("2x_" + to_string(i + 1 - j) + "x_" + to_string(i + j));
        for (int j = 0; j <= n - 2 * i - 2; ++j) poly += MM("2x_" + to_string(j) + "x_" + to_string(j + 2 * i + 1));
        ideal.insert(poly);
    }
    for (int i = 0; i < n / 2 - 1; ++i) {
        PMFR poly("x_" + to_string(i + 1) + "^2 - x_" + to_string(2 * i + 2));
        for (int j = 1; j <= i; ++j) poly += MM("2x_" + to_string(i + 1 - j) + "x_" + to_string(i + j + 1));
        for (int j = 0; j <= n - 2 * i - 3; ++j) poly += MM("2x_" + to_string(j) + "x_" + to_string(j + 2 * i + 2));
        ideal.insert(poly);
    }
    ideal.make_reduced_groebner_basis();
    auto finish_time = chrono::steady_clock::now();
    auto work_time = chrono::duration_cast<chrono::microseconds>(finish_time - start_time).count();
    cout << "N = " << n << endl;
    cout << "Ideal size: " << ideal.size() << endl;
    cout << "Time: " << work_time / 1e6 << " s" << endl;
}

int main() {
    for (int n = 2; n <= 7; ++n) { test_katsura(n); }
}

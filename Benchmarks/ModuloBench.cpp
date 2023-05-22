#pragma GCC optimize("O2")

#include <chrono>
#include <iostream>
#include <random>

namespace {
    using namespace std;

    //int MOD = 1e9+9;
    int MOD = 998244353;
    const int ITER = 1e8;
    int a1[ITER], a2[ITER];

    void gen_vals() {
        uniform_int_distribution<int> gen(0, MOD - 1);
        mt19937 rng(777);
        for (auto& x : a1) x = gen(rng);
        for (auto& x : a2) x = gen(rng);
    }

    void bench_straightforward_modulo() {
        auto start = std::chrono::steady_clock::now();
        int hash = 0;
        for (int i = 0; i < ITER; ++i) {
            int x = a1[i], y = a2[i];
            int s = (x + y) % MOD;
            hash ^= s;
        }
        auto finish = std::chrono::steady_clock::now();
        cout << "Hash: " << hash << endl;
        cout << "Straightforward time: "
             << std::chrono::duration_cast<std::chrono::microseconds>(finish - start).count() / 1e6 << " s" << endl;
    }

    void bench_optimized_modulo() {
        auto start = std::chrono::steady_clock::now();
        int hash = 0;
        for (int i = 0; i < ITER; ++i) {
            int x = a1[i], y = a2[i];
            int s = x + y;
            s -= s < MOD ? 0 : MOD;
            hash ^= s;
        }
        auto finish = std::chrono::steady_clock::now();
        cout << "Hash: " << hash << endl;
        cout << "Optimized time: "
             << std::chrono::duration_cast<std::chrono::microseconds>(finish - start).count() / 1e6 << " s" << endl;
    }
}// namespace

int main() {
    gen_vals();
    bench_straightforward_modulo();
    bench_optimized_modulo();
}

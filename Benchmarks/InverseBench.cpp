//#pragma GCC optimize("O2")

#include <chrono>
#include <iostream>
#include <random>

namespace {
    using namespace std;

    const int64_t MOD = 666667;
    //int64_t MOD = 998244353;
    const int ITER = 1e7;
    int a[ITER];

    void gen_vals() {
        uniform_int_distribution<int> gen(1, MOD - 1);
        mt19937 rng(777);
        for (auto& x : a) x = gen(rng);
    }

    template<typename T>
    T binpow(T val, T pow) {
        T res = 1;
        for (T x = val; pow; pow >>= 1) {
            if (pow & 1) { res = res * x % MOD; }
            x = x * x % MOD;
        }
        return res;
    }

    template<typename T>
    T iterative_euclid(T x, T m) {
        if (x == 0) return 0;
        T u = 0, v = 1, stm = m;
        while (x != 0) {
            T t = m / x;
            m -= t * x;
            swap(x, m);
            u -= t * v;
            swap(u, v);
        }
        if (u < 0) u += stm;
        return u;
    }

    template<typename T>
    T recursive_euclid(T a, T m) {
        auto gcd = [](auto&& gcd, T a, T b, T& x, T& y) {
            if (!a) {
                x = 0, y = 1;
                return b;
            }
            T g = gcd(gcd, b % a, a, x, y);
            T nx = y - x * (b / a), ny = x;
            x = nx, y = ny;
            return g;
        };
        T x, y, g = gcd(gcd, a, m, x, y);
        x -= x / m * m;
        if (x < 0) { x += m; }
        return x;
    }

    template<typename T>
    T oneliner_euclid(T x, T m) {
        return x == 1 ? x : (1 - oneliner_euclid(m % x, x) * m) / x + m;
    }

    void bench_binpow() {
        auto start = std::chrono::steady_clock::now();
        int64_t hash = 0;
        for (int i = 0; i < ITER; ++i) { hash ^= binpow<int64_t>(a[i], MOD - 2); }
        auto finish = std::chrono::steady_clock::now();
        cout << "Hash: " << hash << endl;
        cout << "Binpow time: " << std::chrono::duration_cast<std::chrono::microseconds>(finish - start).count() / 1e6
             << " s" << endl;
    }

    void bench_extended_euclid_iterative() {
        auto start = std::chrono::steady_clock::now();
        int64_t hash = 0;
        for (int i = 0; i < ITER; ++i) { hash ^= iterative_euclid<int64_t>(a[i], MOD); }
        auto finish = std::chrono::steady_clock::now();
        cout << "Hash: " << hash << endl;
        cout << "Euclid iterative time: "
             << std::chrono::duration_cast<std::chrono::microseconds>(finish - start).count() / 1e6 << " s" << endl;
    }

    void bench_extended_euclid_recursive() {
        auto start = std::chrono::steady_clock::now();
        int64_t hash = 0;
        for (int i = 0; i < ITER; ++i) { hash ^= recursive_euclid<int64_t>(a[i], MOD); }
        auto finish = std::chrono::steady_clock::now();
        cout << "Hash: " << hash << endl;
        cout << "Euclid recursive time: "
             << std::chrono::duration_cast<std::chrono::microseconds>(finish - start).count() / 1e6 << " s" << endl;
    }

    void bench_oneliner_euclid() {
        auto start = std::chrono::steady_clock::now();
        int64_t hash = 0;
        for (int i = 0; i < ITER; ++i) { hash ^= oneliner_euclid<int64_t>(a[i], MOD); }
        auto finish = std::chrono::steady_clock::now();
        cout << "Hash: " << hash << endl;
        cout << "Euclid oneliner time: "
             << std::chrono::duration_cast<std::chrono::microseconds>(finish - start).count() / 1e6 << " s" << endl;
    }
}// namespace

int main() {
    gen_vals();
    bench_binpow();
    bench_extended_euclid_iterative();
    bench_extended_euclid_recursive();
    bench_oneliner_euclid();
}

#pragma once
#include <cassert>
#include <limits>
#include <ostream>
#include <type_traits>

template<typename T, const T MOD>
class Mint {
    static_assert(std::is_integral_v<T> && std::is_signed_v<T>, "Numeric type must be signed and integral");
    static_assert(MOD > 0, "MOD must be positive");
    static_assert(std::numeric_limits<T>::max() / MOD > MOD, "MOD^2 should fit in numeric type");

public:
    Mint() = default;

    template<typename U>
    Mint(U value) : value_(reduce(value)) {
        static_assert(std::is_integral_v<U> && std::is_signed_v<U>, "Numeric type must be signed and integral");
    }

    Mint& operator+=(const Mint& rhs) {
        value_ += rhs.value_;
        value_ -= value_ < MOD ? 0 : MOD;
        return *this;
    }
    friend Mint operator+(const Mint& lhs, const Mint& rhs) {
        Mint res = lhs;
        res += rhs;
        return res;
    }

    Mint& operator-=(const Mint& rhs) {
        value_ -= rhs.value_;
        value_ += value_ < 0 ? MOD : 0;
        return *this;
    }
    friend Mint operator-(const Mint& lhs, const Mint& rhs) {
        Mint res = lhs;
        res -= rhs;
        return res;
    }

    Mint& operator*=(const Mint& rhs) {
        value_ *= rhs.value_;
        value_ %= MOD;
        return *this;
    }
    friend Mint operator*(const Mint& lhs, const Mint& rhs) {
        Mint res = lhs;
        res *= rhs;
        return res;
    }

    Mint& operator/=(const Mint& rhs) {
        assert(rhs.value_ != 0 && "Division by 0!");
        value_ *= binpow(rhs.value_, MOD - 2);
        value_ %= MOD;
        return *this;
    }
    friend Mint operator/(const Mint& lhs, const Mint& rhs) {
        Mint res = lhs;
        res /= rhs;
        return res;
    }

    Mint operator-() const { return value_ ? MOD - value_ : 0; }

    bool operator==(const Mint& rhs) const { return value_ == rhs.value_; }
    friend bool operator!=(const Mint& lhs, const Mint& rhs) { return !(lhs == rhs); }
    bool operator<(const Mint& rhs) const { return value_ < rhs.value_; }
    friend bool operator>(const Mint& lhs, const Mint& rhs) { return rhs < lhs; }
    friend bool operator<=(const Mint& lhs, const Mint& rhs) { return !(rhs < lhs); }
    friend bool operator>=(const Mint& lhs, const Mint& rhs) { return !(lhs < rhs); }

    void pow(T power) { value_ = binpow(value_, power); }
    friend Mint pow(const Mint& rhs, T power) {
        Mint res = rhs;
        res.pow(power);
        return res;
    }

    void invert() {
        assert(value_ && "Attempt to invert 0.");
        value_ = binpow(value_, MOD - 2);
    }
    friend Mint invert(const Mint& rhs) {
        Mint res = rhs;
        res.invert();
        return res;
    }

    friend std::ostream& operator<<(std::ostream& out, const Mint& rhs) { return out << rhs.value_; }

private:
    static T binpow(T val, T pow) {
        if (pow < 0) { return binpow(binpow(val, MOD - 2), -pow); }
        T res = 1;
        for (T x = val; pow; pow >>= 1) {
            if (pow & 1) { res = res * x % MOD; }
            x = x * x % MOD;
        }
        return res;
    }

    static T reduce(T c) {
        if (c < 0) {
            c += ((-c - 1) / MOD + 1) * MOD;
        } else if (c >= MOD) {
            c %= MOD;
        }
        return c;
    }

    T value_ = 0;
};

#pragma once
#include <algorithm>
#include <cassert>
#include <numeric>
#include <stdexcept>

template<typename T = int64_t>
class Fraction {
    static_assert(std::is_integral_v<T> && std::is_signed_v<T>, "Numeric type must be signed and integral");

public:
    Fraction() = default;

    template<typename U>
    Fraction(U value) : numerator_(value) {
        static_assert(std::is_integral_v<U> && std::is_signed_v<U>, "Numeric type must be signed and integral");
    }

    template<typename U>
    Fraction(U numerator, U denominator) : numerator_(numerator), denominator_(denominator) {
        static_assert(std::is_integral_v<U> && std::is_signed_v<U>, "Numeric type must be signed and integral");
        assert(denominator_ && "Fraction can't have a zero denominator");
        normalize();
    }

    Fraction& operator+=(const Fraction& rhs) {
        assert(denominator_ && rhs.denominator_ && "Fraction can't have a zero denominator");
        numerator_ = numerator_ * rhs.denominator_ + rhs.numerator_ * denominator_;
        denominator_ *= rhs.denominator_;
        normalize();
        return *this;
    }
    friend Fraction operator+(const Fraction& lhs, const Fraction& rhs) {
        Fraction res = lhs;
        res += rhs;
        return res;
    }

    Fraction& operator-=(const Fraction& rhs) {
        assert(denominator_ && rhs.denominator_ && "Fraction can't have a zero denominator");
        numerator_ = numerator_ * rhs.denominator_ - rhs.numerator_ * denominator_;
        denominator_ *= rhs.denominator_;
        normalize();
        return *this;
    }
    friend Fraction operator-(const Fraction& lhs, const Fraction& rhs) {
        Fraction res = lhs;
        res -= rhs;
        return res;
    }

    Fraction& operator*=(const Fraction& rhs) {
        assert(denominator_ && rhs.denominator_ && "Fraction can't have a zero denominator");
        numerator_ *= rhs.numerator_;
        denominator_ *= rhs.denominator_;
        normalize();
        return *this;
    }
    friend Fraction operator*(const Fraction& lhs, const Fraction& rhs) {
        Fraction res = lhs;
        res *= rhs;
        return res;
    }

    Fraction& operator/=(const Fraction& rhs) {
        assert(rhs.numerator_ && "Division by zero!");
        assert(denominator_ && rhs.denominator_ && "Fraction can't have a zero denominator");
        numerator_ *= rhs.denominator_;
        denominator_ *= rhs.numerator_;
        normalize();
        return *this;
    }
    friend Fraction operator/(const Fraction& lhs, const Fraction& rhs) {
        Fraction res = lhs;
        res /= rhs;
        return res;
    }

    Fraction operator-() const { return {-numerator_, denominator_}; }

    bool operator==(const Fraction& rhs) const {
        return numerator_ == rhs.numerator_ && denominator_ == rhs.denominator_;
    }
    friend bool operator!=(const Fraction& lhs, const Fraction& rhs) { return !(lhs == rhs); }
    bool operator<(const Fraction& rhs) const { return numerator_ * rhs.denominator_ < rhs.numerator_ * denominator_; }
    friend bool operator>(const Fraction& lhs, const Fraction& rhs) { return rhs < lhs; }
    friend bool operator<=(const Fraction& lhs, const Fraction& rhs) { return !(rhs < lhs); }
    friend bool operator>=(const Fraction& lhs, const Fraction& rhs) { return !(lhs < rhs); }

    void pow(T power) {
        if (numerator_ == 0) { return; }
        assert(denominator_ > 0);
        T sign = numerator_ > 0 ? 1 : -1;
        numerator_ *= sign;
        if (power < 0) {
            std::swap(numerator_, denominator_);
            power *= -1;
        }
        numerator_ = binpow(numerator_, power);
        denominator_ = binpow(denominator_, power);
        if ((power & 1) && sign == -1) { numerator_ *= -1; }
    }
    friend Fraction pow(const Fraction& rhs, T power) {
        Fraction res = rhs;
        res.pow(power);
        return res;
    }

    friend std::ostream& operator<<(std::ostream& os, const Fraction& f) {
        if (f.denominator_ == 1) { return os << f.numerator_; }
        return os << std::string("\\frac{") << f.numerator_ << "}{" << f.denominator_ << "}";
    }

    void invert() {
        assert(numerator_ != 0 && "Can't invert 0");
        std::swap(numerator_, denominator_);
    }
    friend Fraction invert(const Fraction& rhs) {
        Fraction res = rhs;
        res.invert();
        return res;
    }

private:
    static T sign(T x) { return x < 0 ? -1 : x == 0 ? 0 : 1; }

    static bool is_sum_overflow(T x, T y) {
        if (sign(x) != sign(y) || sign(x) == 0) { return false; }
        if (sign(x) == 1) { return y > std::numeric_limits<T>::max() - x; }
        return y < std::numeric_limits<T>::min() - x;
    }

    static bool is_prod_overflow(T x, T y) {
        if (x == 0 || y == 0) { return false; }
        T res = x * y;
        return res / x != y || res / y != x;
    }

    static T binpow(T val, T pow) {
        assert(pow >= 0 && "Negative power is not allowed!");
        T res = 1;
        for (T x = val; pow; pow >>= 1) {
            if (pow & 1) { res *= x; }
            x *= x;
        }
        return res;
    }

    void normalize() {
        assert(denominator_ != 0);
        if (denominator_ < 0) {
            denominator_ *= -1;
            numerator_ *= -1;
        }
        T gcd = std::gcd(numerator_, denominator_);
        numerator_ /= gcd;
        denominator_ /= gcd;
    }

    T numerator_ = 0;
    T denominator_ = 1;
};

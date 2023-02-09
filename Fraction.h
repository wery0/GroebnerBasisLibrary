#pragma once
#include <algorithm>
#include <cassert>
#include <stdexcept>

template<typename T = int64_t>
class Fraction {
    static_assert(std::is_integral_v<T> && std::is_signed_v<T>, "Numeric type must be signed and integral");

public:
    Fraction() = default;

    template<typename U>
    Fraction(U n) : numerator_(n) {
        static_assert(std::is_integral_v<U> && std::is_signed_v<U>, "Numeric type must be signed and integral");
    }

    template<typename U>
    Fraction(U numerator, U denominator) : numerator_(numerator), denominator_(denominator) {
        static_assert(std::is_integral_v<U> && std::is_signed_v<U>, "Numeric type must be signed and integral");
        assert(denominator_ && "Fraction can't have a zero denominator");
        normalize();
    }

    Fraction& operator+=(const Fraction& rhs) {
        assert(rhs.denominator_ && "Fraction can't have a zero denominator");
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
        assert(rhs.denominator_ && "Fraction can't have a zero denominator");
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
        assert(rhs.denominator_ && "Fraction can't have a zero denominator");
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
        assert(rhs.denominator_ && "Fraction can't have a zero denominator");
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

    bool operator==(const Fraction& rhs) const { return ((*this) - rhs).numerator_ == 0; }
    bool operator<(const Fraction& rhs) const { return ((*this) - rhs).numerator_ < 0; }
    friend bool operator!=(const Fraction& lhs, const Fraction& rhs) { return !(lhs == rhs); }
    friend bool operator>(const Fraction& lhs, const Fraction& rhs) { return rhs < lhs; }
    friend bool operator<=(const Fraction& lhs, const Fraction& rhs) { return lhs < rhs || lhs == rhs; }
    friend bool operator>=(const Fraction& lhs, const Fraction& rhs) { return rhs < lhs || lhs == rhs; }

    void pow(T power) { binpow(numerator_, denominator_, power); }
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
        normalize();
    }
    friend Fraction invert(const Fraction& rhs) {
        Fraction res = rhs;
        res.invert();
        return res;
    }

private:
    static int sign(T x) { return x < 0 ? -1 : x == 0 ? 0 : 1; }

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

    static void binpow(T& numerator, T& denominator, T pow) {
        if (numerator == 0) { return; }
        assert(denominator > 0);
        T sign = numerator > 0 ? 1 : -1;
        numerator *= sign;
        if (pow < 0) {
            std::swap(numerator, denominator);
            pow *= -1;
        }
        numerator = binpow(numerator, pow);
        denominator = binpow(denominator, pow);
        if ((pow & 1) && sign == -1) { numerator *= -1; }
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
        T gc = std::__gcd(abs(numerator_), denominator_);
        numerator_ /= gc;
        denominator_ /= gc;
    }

    T numerator_ = 0;
    T denominator_ = 1;
};
#pragma once
#include <stdexcept>

template<typename T = int64_t>
class Fraction {
    static_assert(std::is_integral_v<T>, "Number type must be an integral type");

public:
    Fraction() = default;

    template<typename U>
    Fraction(U n) : numerator_(n) {
        static_assert(std::is_integral_v<U>, "Number type must be an integral type");
    }

    template<typename U>
    Fraction(U num, U den) : numerator_(num), denominator_(den) {
        static_assert(std::is_integral_v<U>, "Number type must be an integral type");
        normalize();
    }

    Fraction operator-() const { return {-numerator_, denominator_}; }
    Fraction operator+(const Fraction& rhs) const {
        return {numerator_ * rhs.denominator_ + rhs.numerator_ * denominator_, denominator_ * rhs.denominator_};
    }
    Fraction operator-(const Fraction& rhs) const {
        return {numerator_ * rhs.denominator_ - rhs.numerator_ * denominator_, denominator_ * rhs.denominator_};
    }
    Fraction operator*(const Fraction& rhs) const {
        return {numerator_ * rhs.numerator_, denominator_ * rhs.denominator_};
    }
    Fraction operator/(const Fraction& rhs) const {
        if (rhs.numerator_ == 0) { throw std::runtime_error("Fraction division by 0!"); }
        return {numerator_ * rhs.denominator_, denominator_ * rhs.numerator_};
    }

    void operator+=(const Fraction& rhs) {
        (*this) = (*this) + rhs;
        normalize();
    }

    void operator-=(const Fraction& rhs) {
        (*this) = (*this) - rhs;
        normalize();
    }

    void operator*=(const Fraction& rhs) {
        (*this) = (*this) * rhs;
        normalize();
    }

    void operator/=(const Fraction& rhs) {
        (*this) = (*this) / rhs;
        normalize();
    }

    bool operator==(const Fraction& rhs) const { return ((*this) - rhs).numerator_ == 0; }
    bool operator!=(const Fraction& rhs) const { return ((*this) - rhs).numerator_ != 0; }
    bool operator<(const Fraction& rhs) const { return ((*this) - rhs).numerator_ < 0; }
    bool operator>(const Fraction& rhs) const { return ((*this) - rhs).numerator_ > 0; }

    template<class ostream>
    friend ostream& operator<<(ostream& os, const Fraction& f) {
        if (f.denominator_ == 1) { return os << f.numerator_; }
        return os << "\\frac{" << f.numerator_ << "}{" << f.denominator_ << "}";
    }

    Fraction get_inv() { return {denominator_, numerator_}; }

    friend Fraction inv(const Fraction& f) { return {f.den, f.num}; }
    friend Fraction abs(const Fraction& f) { return f > 0 ? f : -f; }

private:
    T abs(T x) { return x < 0 ? -x : x; }

    T gcd(T x, T y) {
        x = abs(x);
        y = abs(y);
        while (x > 0 && y > 0) {
            x %= y;
            std::swap(x, y);
        }
        return x + y;
    }

    void normalize() {
        if (denominator_ < 0) {
            denominator_ *= -1;
            numerator_ *= -1;
        }
        T gc = gcd(abs(numerator_), denominator_);
        numerator_ /= gc;
        denominator_ /= gc;
    }

    T numerator_ = 0;
    T denominator_ = 1;
};
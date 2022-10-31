#include <limits>

template<typename T, const T MOD>
class Mint {
    static_assert(std::is_integral_v<T>, "Number type must be an integral type");
    static_assert(MOD > 0, "MOD must be positive");
    static_assert(std::numeric_limits<T>::max() / MOD > MOD, "MOD^2 should fit in NumberType");

public:
    Mint() = default;

    template<typename U>
    Mint(U c) {
        static_assert(std::is_integral_v<U>, "Number type must be an integral type");
        if (c < 0) {
            c += ((-c - 1) / MOD + 1) * MOD;
        } else if (c >= MOD) {
            c %= MOD;
        }
        val_ = c;
    }

    Mint operator+(const Mint& rhs) const { return val_ + rhs.val_ >= MOD ? val_ + rhs.val_ - MOD : val_ + rhs.val_; }
    Mint operator-(const Mint& rhs) const { return val_ < rhs.val_ ? MOD - rhs.val_ + val_ : val_ - rhs.val_; }
    Mint operator*(const Mint& rhs) const { return val_ * rhs.val_ % MOD; }
    Mint operator/(const Mint& rhs) const { return val_ * inv(rhs).val_ % MOD; }
    Mint operator-() const { return MOD - val_; }

    void operator+=(const Mint& rhs) {
        val_ += rhs.val_;
        val_ -= val_ < MOD ? 0 : MOD;
    }
    void operator-=(const Mint& rhs) {
        val_ += MOD - rhs.val_;
        val_ -= val_ < MOD ? 0 : MOD;
    }
    void operator*=(const Mint& rhs) { val_ = val_ * rhs.val_ % MOD; }
    void operator/=(const Mint& rhs) { val_ = val_ * inv(rhs).val_ % MOD; }

    Mint& operator++() {
        val_ = val_ == MOD - 1 ? 0 : val_ + 1;
        return *this;
    }
    Mint operator++(int) {
        Mint res = *this;
        ++*this;
        return res;
    }

    bool operator==(const Mint& rhs) const { return val_ == rhs.val_; }
    bool operator!=(const Mint& rhs) const { return val_ != rhs.val_; }
    bool operator<(const Mint& rhs) const { return val_ < rhs.val_; }
    bool operator>(const Mint& rhs) const { return val_ > rhs.val_; }


    Mint get_pow(T pw) const {
        T res = 1;
        for (T x = val_; pw; pw >>= 1) {
            if (pw & 1) { res = res * x % MOD; }
            x = x * x % MOD;
        }
        return res;
    }

    Mint get_inv(const Mint& m) const { return get_pow(MOD - 2); }

    friend Mint pow(const Mint& m, T pw) { return m.get_pow(pw); }
    friend Mint inv(const Mint& m) { return m.get_pow(MOD - 2); }

    template<class ostream>
    friend ostream& operator<<(ostream& out, const Mint& m) {
        return out << m.val_;
    }

private:
    T val_ = 0;
};
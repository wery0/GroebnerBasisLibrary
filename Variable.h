#pragma once
#include <cassert>
#include <cctype>
#include <cstdint>
#include <tuple>

//Variable should look like [A-Za-z](_(\d)+)?
//Examples: a, Y, x_0, P_314159
template<typename NumberType = int32_t>
class Variable {
public:
    Variable() = default;

    explicit Variable(char letter, NumberType number = -1) : letter_(letter), number_(number) {
        if (!isalpha(letter)) {
            std::string err_msg = "Expected alpha letter_ for variable name, but got: ";
            err_msg += letter;
            throw std::runtime_error(err_msg);
        }
    }

    bool operator==(const Variable& rhs) const { return letter_ == rhs.letter_ && number_ == rhs.number_; }

    bool operator!=(const Variable& rhs) const { return letter_ != rhs.letter_ || number_ != rhs.number_; }

    bool operator<(const Variable& rhs) const {
        std::pair<char, NumberType> t1 = {letter_, number_};
        std::pair<char, NumberType> t2 = {rhs.letter_, rhs.number_};
        return t1 < t2;
    }

    template<class ostream>
    friend ostream& operator<<(ostream& os, const Variable& v) {
        if (v.number_ == -1) { return os << v.letter_; }
        return os << v.letter_ << '_' << '{' << v.number_ << '}';
    }

private:
    char letter_;
    NumberType number_ = -1;
};
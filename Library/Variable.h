#pragma once
#include "../Orders/VariableOrders.h"
#include <cassert>
#include <cctype>
#include <cstdint>
#include <tuple>

//Variable should look like [A-Za-z](_(\d)+)?
//Examples: a, Y, x_0, P_314159
template<typename NumberType = int32_t, typename VariableOrder = VariableOrders::AsciiOrder>
class Variable {
    static constexpr NumberType kNoIndex = -1;

public:
    Variable() = default;

    explicit Variable(char letter, NumberType number = kNoIndex) : letter_(letter), number_(number) {
        assert(('a' <= letter && letter <= 'z') || ('A' <= letter && letter <= 'Z') && "Expected [A-Za-z] letter");
    }

    bool operator==(const Variable& rhs) const { return letter_ == rhs.letter_ && number_ == rhs.number_; }
    friend bool operator!=(const Variable& lhs, const Variable& rhs) { return !(lhs == rhs); }

    bool operator<(const Variable& rhs) const {
        static VariableOrder var_order;
        if (letter_ != rhs.letter_) { return var_order(letter_, rhs.letter_); }
        return number_ > rhs.number_;
    }

    friend std::ostream& operator<<(std::ostream& os, const Variable& v) {
        if (v.number_ == kNoIndex) { return os << v.letter_; }
        return os << v.letter_ << "_" << v.number_;
    }

private:
    char letter_;
    NumberType number_;
};

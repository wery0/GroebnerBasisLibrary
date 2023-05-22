#pragma once

namespace VariableOrders {
    struct AsciiOrder {
        bool operator()(const char c1, const char c2) { return c1 < c2; }
    };

    struct InverseAsciiOrder {
        bool operator()(const char c1, const char c2) { return c1 > c2; }
    };
}// namespace VariableOrders

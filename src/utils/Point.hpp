#pragma once

#include <algorithm>
#include <array>
#include <iterator>

namespace utils {
using Point = std::array<float, 3>;

inline Point& operator+(Point& lhs, Point& rhs) {
    std::transform(std::begin(lhs), std::end(lhs), std::begin(lhs),
                   std::begin(lhs), std::plus<void>());
    return lhs;
}

inline Point& operator+(Point& lhs, float scalar) {
    for (auto& val : lhs) {
        val += scalar;
    }
    return lhs;
}

inline Point& operator*(Point& lhs, float scalar) {
    for (auto& val : lhs) {
        val *= scalar;
    }
    return lhs;
}
} // namespace utils

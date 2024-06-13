#pragma once

#include <array>

namespace utils
{
struct Point : std::array<float, 3> {
    Point& operator+=(const Point& rhs);

    Point operator-(const Point& rhs) const;
    Point operator*(float scalar) const;

    float module() const;
};

inline constexpr Point zeroPoint{0, 0, 0};
} // namespace utils

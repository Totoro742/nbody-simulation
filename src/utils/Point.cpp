#include "utils/Point.hpp"

#include <algorithm>
#include <cmath>
#include <iterator>
#include <numeric>

namespace utils
{
inline Point& Point::operator+=(const Point& rhs)
{
    Point& lhs{*this};
    std::transform(std::begin(lhs), std::end(lhs), std::begin(rhs),
                   std::begin(lhs), std::plus<void>());
    return lhs;
}

inline Point Point::operator-(const Point& rhs) const
{
    const Point& lhs{*this};
    Point result{};
    std::transform(std::begin(lhs), std::end(lhs), std::begin(rhs),
                   std::begin(result), std::minus<void>());
    return result;
}

inline Point Point::operator*(float scalar) const
{
    const Point& lhs{*this};
    Point result{};
    std::transform(std::begin(lhs), std::end(lhs), std::begin(result),
                   [scalar](const auto elem) { return elem * scalar; });
    return result;
}

inline float Point::module() const
{
    const float sumOfSquares{
        std::accumulate(std::begin(*this), std::end(*this), 0.0f,
                        [](const float sum, const float value) {
                            return sum + value * value;
                        })};
    return std::sqrt(sumOfSquares);
}
} // namespace utils

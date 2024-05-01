#pragma once

#include <array>
#include <mpi.h>

namespace utils
{
struct Point : std::array<float, 3> {
    Point& operator+=(const Point& rhs);

    Point operator-(const Point& rhs) const;
    Point operator*(float scalar) const;

    float module() const;

    static MPI::Datatype mpiType() { return MPI::FLOAT.Create_contiguous(3); };
};

inline constexpr Point zeroPoint{0, 0, 0};
} // namespace utils

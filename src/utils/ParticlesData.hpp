#pragma once

#include "utils/Point.hpp"
#include <vector>

namespace utils
{
using PointVector = std::vector<Point>;

struct ParticlesData {
    PointVector positions;
    PointVector velocities;
    std::vector<float> masses;
};
} // namespace utils

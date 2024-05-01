#pragma once

#include "utils/Point.hpp"
#include <vector>

namespace utils
{
struct ParticlesData {
    std::vector<Point> positions;
    std::vector<Point> velocities;
    std::vector<float> masses;
};
} // namespace utils

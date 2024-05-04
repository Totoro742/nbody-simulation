#include "algorithms/Leapfrog7.hpp"

#include "utils/Point.hpp"
#include <cmath>
#include <cstdio>

using utils::Point;

namespace
{
Point calcAcceleration(const Point& planet,
                       const Point& star,
                       const float starMass)
{
    constexpr float G{6.67430e-11};
    const Point diff{planet - star};
    const float distance{diff.module()};
    return diff * (-G * starMass * std::pow(distance, -3));
}
} // namespace

namespace algorithms
{
void Leapfrog7::updatePositions(const float miniTimeStep)
{
    for (int idx{0}; idx < positions_view.size(); idx++) {
        positions_view[idx] += data.velocities[idx] * miniTimeStep;
    }
}

void Leapfrog7::updateVelocities(const float miniTimeStep)
{
    for (int idx{0}; idx < positions_view.size(); idx++) {
        data.velocities[idx] +=
            calcTotalAcceleration(range.first + idx) * miniTimeStep;
    }
}

utils::Point Leapfrog7::calcTotalAcceleration(const int planetIdx) const
{
    utils::Point acceleration{utils::zeroPoint};
    const Point& planet{data.positions[planetIdx]};

    for (int starIdx{0}; starIdx < planetIdx; starIdx++) {
        const Point& star{data.positions[starIdx]};
        const float starMass{data.masses[starIdx]};
        acceleration += calcAcceleration(planet, star, starMass);
    }

    for (int starIdx{0}; starIdx < data.positions.size(); starIdx++) {
        const Point& star{data.positions[starIdx]};
        const float starMass{data.masses[starIdx]};
        acceleration += calcAcceleration(planet, star, starMass);
    }

    const auto size{data.positions.size()};
    for (int starIdx{planetIdx + 1}; starIdx < size; starIdx++) {
        const Point& star{data.positions[starIdx]};
        const float starMass{data.masses[starIdx]};
        acceleration += calcAcceleration(planet, star, starMass);
    }

    return acceleration;
}
} // namespace algorithms

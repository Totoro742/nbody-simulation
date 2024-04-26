#include "algorithms/Leapfrog7.hpp"

#include "utils/Point.hpp"
#include <algorithm>
#include <cmath>
#include <iterator>
#include <numbers>

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
void Leapfrog7::performeStep()
{
    constexpr auto w{std::numbers::sqrt2};
    constexpr auto f{2 - w};

    updatePositions(timeStep / (2 * f));
    updateVelocities(timeStep / f);
    updatePositions(timeStep * ((1 - w) / 2 * f));
    updateVelocities(timeStep * (-w / f));
    updatePositions(timeStep * ((1 - w) / 2 * f));
    updateVelocities(timeStep / f);
    updatePositions(timeStep / (2 * f));
}

void Leapfrog7::updatePositions(const float miniTimeStep)
{
    for (int idx{0}; idx < positions.size(); idx++) {
        positions[idx] += velocities[idx] * miniTimeStep;
    }
}

void Leapfrog7::updateVelocities(const float miniTimeStep)
{
    updateAcceleration();

    for (int idx{0}; idx < velocities.size(); idx++) {
        velocities[idx] += accelerations[idx] * miniTimeStep;
    }
}

void Leapfrog7::updateAcceleration()
{
    std::fill(std::begin(accelerations), std::end(accelerations),
              utils::zeroPoint);

    for (int starIdx{0}; starIdx < positions.size(); starIdx++) {
        const Point& star{positions[starIdx]};
        const float starMass{masses[starIdx]};

        for (int planetIdx{0}; planetIdx < starIdx; planetIdx++) {
            const Point& planet{positions[planetIdx]};
            accelerations[planetIdx] +=
                calcAcceleration(planet, star, starMass);
        }

        for (int planetIdx{starIdx}; planetIdx < positions.size();
             planetIdx++) {
            const Point& planet{positions[planetIdx]};
            accelerations[planetIdx] +=
                calcAcceleration(planet, star, starMass);
        }
    }
}
} // namespace algorithms

#pragma once

#include "utils/ParticlesData.hpp"
#include "utils/Point.hpp"
#include <numbers>
#include <ranges>
#include <vector>

namespace algorithms
{
class Leapfrog7
{
public:
    Leapfrog7(utils::ParticlesData& data, std::pair<int, int> range)
        : data{data}, range{range}
    {
    }

    template <std::invocable<std::vector<utils::Point>&> ShareFunction>
    void performStep(const float timeStep, const ShareFunction shareFunction)
    {
        constexpr auto w{std::numbers::sqrt2};
        constexpr auto f{2 - w};

        updatePositions(timeStep / (2 * f));

        shareFunction(data.positions);
        updateVelocities(timeStep / f);
        updatePositions(timeStep * ((1 - w) / 2 * f));

        shareFunction(data.positions);
        updateVelocities(timeStep * (-w / f));
        updatePositions(timeStep * ((1 - w) / 2 * f));

        shareFunction(data.positions);
        updateVelocities(timeStep / f);
        updatePositions(timeStep / (2 * f));
    }

private:
    void updatePositions(const float miniTimeStep);
    void updateVelocities(const float miniTimeStep);
    utils::Point calcTotalAcceleration(const int planetIdx) const;

    utils::ParticlesData& data;
    std::pair<int, int> range;
    std::ranges::subrange<std::vector<utils::Point>::iterator> positions_view{
        data.positions.begin() + range.first,
        data.positions.begin() + range.second};
};
} // namespace algorithms

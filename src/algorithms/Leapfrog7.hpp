#pragma once

#include "utils/Point.hpp"
#include <vector>

namespace algorithms
{
class Leapfrog7
{
  public:
    Leapfrog7(std::vector<utils::Point>&& positions,
              std::vector<utils::Point>&& velocities,
              std::vector<float> masses,
              // std::pair<std::size_t, std::size_t> range,
              const float timeStep)
        : positions{positions}, velocities{velocities},
          accelerations(positions.size()), masses{masses},
          // range{range},
          timeStep{timeStep}
    {
    }

    void performeStep();

  private:
    void updatePositions(const float miniTimeStep);
    void updateVelocities(const float miniTimeStep);
    void updateAcceleration();

    std::vector<utils::Point> positions;
    std::vector<utils::Point> velocities;
    std::vector<utils::Point> accelerations;
    std::vector<float> masses;
    // std::pair<std::size_t, std::size_t> range;
    const float timeStep;
};
} // namespace algorithms

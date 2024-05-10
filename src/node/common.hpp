#pragma once

#include "algorithms/Leapfrog7.hpp"
#include "node/NodeConfig.hpp"
#include "utils/MpiDatatypeRAII.hpp"
#include "utils/ParticlesData.hpp"
#include "utils/SimParams.hpp"
#include <mpi.h>
#include <optional>

namespace node::common
{
NodeConfig
createNodeConfig(const MPI::Comm& comm,
                 const std::optional<int> totalParticles = std::nullopt);

/* initialize particles data between nodes
 */
void initialShareData(const MPI::Comm& comm,
                      const NodeConfig& config,
                      utils::SimParams& simParams,
                      utils::ParticlesData& data);

/* initialize particles data between nodes
 */
inline void initialShareData(const MPI::Comm& comm,
                             const NodeConfig& config,
                             const utils::SimParams& simParams,
                             utils::ParticlesData& data)
{
    auto simParamsCopy{simParams};
    initialShareData(comm, config, simParamsCopy, data);
};

/**
 * Performs distributed algorithm across `comm` based on passed `config`,
 * `params` and `data`.
 * After each iteration `callback` is executed
 */
template <std::invocable<utils::PointVector&, const int> Callback>
void performAlgorithm(const MPI::Comm& comm,
                      const NodeConfig& config,
                      const utils::SimParams simParams,
                      utils::ParticlesData& data,
                      const Callback& callback)
{
    utils::MpiDatatypeRAII pointMpiType{utils::Point::mpiType()};

    const auto offset{config.offsetPerNode[config.nodeRank]};
    algorithms::Leapfrog7 leapfrog{data,
                                   {offset, offset + config.localParticles}};

    const auto shareFunction{[&](std::vector<utils::Point>& positions) {
        comm.Allgatherv(MPI::IN_PLACE, 0, MPI::DATATYPE_NULL, positions.data(),
                        config.particlesPerNode.data(),
                        config.offsetPerNode.data(), pointMpiType);
    }};

    for (int step{0}; step < simParams.iterations; step++) {
        leapfrog.performStep(simParams.timeStep, shareFunction);

        if ((step + 1) % simParams.saveStep == 0) {
            callback(data.positions, step);
        }
    }
}
} // namespace node::common

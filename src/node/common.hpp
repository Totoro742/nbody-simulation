#pragma once

#include "algorithms/Leapfrog7.hpp"
#include "node/NodeConfig.hpp"
#include "utils/ParticlesData.hpp"
#include "utils/Point.hpp"
#include "utils/SimParams.hpp"
#include <cstdio>
#include <optional>
#include <upcxx/upcxx.hpp>

namespace node::common
{
NodeConfig
createNodeConfig(const std::optional<int> totalParticles = std::nullopt);

/*
 * initialize particles data between nodes
 */
void initialShareData(const NodeConfig& config,
                      utils::SimParams& simParams,
                      utils::ParticlesData& data);

/*
 * initialize particles data between nodes
 */
inline void initialShareData(const NodeConfig& config,
                             const utils::SimParams& simParams,
                             utils::ParticlesData& data)
{
    auto simParamsCopy{simParams};
    initialShareData(config, simParamsCopy, data);
};

using DistData = upcxx::dist_object<upcxx::global_ptr<utils::Point>>;

void rputOverDistributed(const utils::PointVector& data,
                         const NodeConfig& config,
                         const DistData& distData);

void rgetOverDistributed(utils::PointVector& data,
                         const NodeConfig& config,
                         const DistData& distData);

/*
 * Performs distributed algorithm based on
 * passed `config`, `simParams`, `data` and `distData`.
 * After each iteration `callback` is executed
 */
template <std::invocable<utils::PointVector&, const int> Callback>
void performAlgorithm(const NodeConfig& config,
                      const utils::SimParams simParams,
                      utils::ParticlesData& data,
                      const DistData& distData,
                      const Callback& callback)
{
    const auto offset{config.offsetPerNode[config.nodeRank]};
    algorithms::Leapfrog7 leapfrog{data,
                                   {offset, offset + config.localParticles}};

    const auto shareFunction{[&](utils::PointVector& positions) {
        const auto local_ptr{distData->local()};
        const auto data_ptr{positions.data() + offset};

        std::copy(data_ptr, data_ptr + config.localParticles, local_ptr);
        rgetOverDistributed(positions, config, distData);
    }};

    for (int step{0}; step < simParams.iterations; step++) {
        leapfrog.performStep(simParams.timeStep, shareFunction);

        if ((step + 1) % simParams.saveStep == 0) {
            callback(data.positions, step);
        }
    }
}
} // namespace node::common

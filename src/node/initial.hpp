#pragma once

#include "node/NodeConfig.hpp"
#include "utils/ParticlesData.hpp"
#include "utils/SimParams.hpp"
#include <mpi.h>
#include <optional>

namespace node::initial
{
node::NodeConfig
createNodeConfig(const MPI::Comm& comm,
                 const std::optional<int> totalParticles = std::nullopt);

void shareData(const MPI::Comm& comm,
               const NodeConfig& config,
               utils::SimParams& simParams,
               utils::ParticlesData& data);

inline void shareData(const MPI::Comm& comm,
                      const NodeConfig& config,
                      const utils::SimParams& simParams,
                      utils::ParticlesData& data)
{
    auto simParamsCopy{simParams};
    shareData(comm, config, simParamsCopy, data);
};
} // namespace node::initial

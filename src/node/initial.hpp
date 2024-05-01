#pragma once

#include "node/NodeConfig.hpp"
#include "utils/ParticlesData.hpp"
#include <mpi.h>
#include <optional>

namespace node::initial
{
node::NodeConfig
createNodeConfig(const MPI::Comm& comm,
                 const std::optional<int> totalParticles = std::nullopt);

void shareData(const MPI::Comm& comm,
               const NodeConfig& config,
               utils::ParticlesData& data);
} // namespace node::initial

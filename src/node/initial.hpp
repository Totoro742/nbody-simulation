#pragma once

#include "node/NodeConfig.hpp"
#include <mpi.h>
#include <optional>

namespace node
{
node::NodeConfig
createNodeConfig(const MPI::Comm& comm,
                 const std::optional<int> totalParticles = std::nullopt);
} // namespace node

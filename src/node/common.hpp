#pragma once

#include "node/NodeConfig.hpp"
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
} // namespace node::common

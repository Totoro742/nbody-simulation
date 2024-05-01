#include "node/initial.hpp"

#include "constants.hpp"
#include "node/NodeConfig.hpp"
#include <cstdio>
#include <mpi.h>
#include <optional>

namespace
{
/* calculate num of local particles that only first/master node has less
 * particles
 */
int calcLocalParticles(const node::NodeConfig& config)
{
    if (config.totalParticles < config.totalNodes) {
        std::fprintf(stderr,
                     "(%d) ERR: number of particles lower than number of nodes "
                     "resulting in UB",
                     config.nodeRank);
    }

    const int particlesToDivide{config.totalParticles + config.totalNodes - 1};
    const int maxLocalParticles{particlesToDivide / config.totalNodes};
    const int lastNodeParticles{particlesToDivide % config.totalNodes};
    return config.nodeRank == masterNodeRank ? lastNodeParticles
                                             : maxLocalParticles;
}
} // namespace

namespace node
{
node::NodeConfig createNodeConfig(const MPI::Comm& comm,
                                  const std::optional<int> totalParticles)
{
    node::NodeConfig config{.totalNodes = config.totalNodes = comm.Get_size(),
                            .nodeRank = config.nodeRank = comm.Get_rank(),
                            .totalParticles = totalParticles.value_or(0)};

    constexpr int oneElement{1};
    comm.Bcast(&config.totalParticles, oneElement, MPI::INT, masterNodeRank);

    config.localParticles = calcLocalParticles(config);

    return config;
};
} // namespace node

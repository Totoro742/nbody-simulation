#include "node/initial.hpp"

#include "constants.hpp"
#include "node/NodeConfig.hpp"
#include "utils/Point.hpp"
#include <cassert>
#include <cstdio>
#include <mpi.h>
#include <optional>
#include <vector>

namespace
{
/* calculate num of local particles that only first/master node has less
 * particles
 */
void calcParticlesPerNode(node::NodeConfig& config)
{
    if (config.totalParticles < config.totalNodes) {
        std::fprintf(stderr,
                     "(%d) ERR: number of particles lower than number of nodes "
                     "resulting in UB\n",
                     config.nodeRank);
    }

    const int particlesToDivide{config.totalParticles + config.totalNodes - 1};
    const int maxLocalParticles{particlesToDivide / config.totalNodes};
    const int masterNodeParticles{particlesToDivide % config.totalNodes};

    config.localParticles = config.nodeRank == masterNodeRank
                                ? masterNodeParticles
                                : maxLocalParticles;

    config.particlesPerNode = std::vector<int>(
        static_cast<size_t>(config.totalNodes), maxLocalParticles);
    config.particlesPerNode[masterNodeRank] = masterNodeParticles;
}
} // namespace

namespace node::initial
{
node::NodeConfig createNodeConfig(const MPI::Comm& comm,
                                  const std::optional<int> totalParticles)
{
    node::NodeConfig config{.totalNodes = config.totalNodes = comm.Get_size(),
                            .nodeRank = config.nodeRank = comm.Get_rank(),
                            .totalParticles = totalParticles.value_or(0)};

    constexpr int oneElement{1};
    comm.Bcast(&config.totalParticles, oneElement, MPI::INT, masterNodeRank);

    if (config.totalParticles != 0) {
        calcParticlesPerNode(config);
    }

    return config;
};

/* share particles data
 */
void shareData(const MPI::Comm& comm,
               const NodeConfig& config,
               utils::SimParams& simParams,
               utils::ParticlesData& data)
{
    assert(data.positions.size() == config.totalParticles);
    assert(data.velocities.size() == config.totalParticles);
    assert(data.masses.size() == config.totalParticles);

    constexpr int oneElement{1};
    comm.Bcast(&simParams.iterations, oneElement, MPI::UNSIGNED_LONG,
               masterNodeRank);
    comm.Bcast(&simParams.timeStep, oneElement, MPI::DOUBLE, masterNodeRank);

    auto pointMpiType{utils::Point::mpiType()};
    pointMpiType.Commit();

    comm.Bcast(data.positions.data(), config.totalParticles, pointMpiType,
               masterNodeRank);
    comm.Bcast(data.velocities.data(), config.totalParticles, pointMpiType,
               masterNodeRank);
    comm.Bcast(data.masses.data(), config.totalParticles, MPI::INT,
               masterNodeRank);

    pointMpiType.Free();
};
} // namespace node::initial

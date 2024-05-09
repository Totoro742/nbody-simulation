#include "node/common.hpp"

#include "constants.hpp"
#include "node/NodeConfig.hpp"
#include "utils/MpiDatatypeRAII.hpp"
#include "utils/Point.hpp"
#include <algorithm>
#include <iterator>
#include <mpi.h>
#include <numeric>
#include <optional>

namespace
{
/* calculate num of local particles that only first/master node has less
 * particles
 */
void calcParticlesPerNode(node::NodeConfig& config)
{
    const int minLocalParticles{config.totalParticles / config.totalNodes};
    const int remainingParticles{config.totalParticles % config.totalNodes};

    config.particlesPerNode.reserve(config.totalNodes);
    std::fill_n(std::back_insert_iterator(config.particlesPerNode),
                config.totalNodes - remainingParticles, minLocalParticles);
    std::fill_n(std::back_insert_iterator(config.particlesPerNode),
                remainingParticles, minLocalParticles + 1);

    config.localParticles = config.particlesPerNode[config.nodeRank];

    config.offsetPerNode.reserve(config.totalNodes);
    std::exclusive_scan(config.particlesPerNode.begin(),
                        config.particlesPerNode.end(),
                        config.offsetPerNode.begin(), 0);
}
} // namespace

namespace node::common
{
node::NodeConfig createNodeConfig(const MPI::Comm& comm,
                                  const std::optional<int> totalParticles)
{
    node::NodeConfig config{.totalNodes = config.totalNodes = comm.Get_size(),
                            .nodeRank = config.nodeRank = comm.Get_rank(),
                            .totalParticles = totalParticles.value_or(0)};

    constexpr int oneElement{1};
    comm.Bcast(&config.totalParticles, oneElement, MPI::INT, masterNodeRank);

    calcParticlesPerNode(config);

    return config;
};

void initialShareData(const MPI::Comm& comm,
                      const NodeConfig& config,
                      utils::SimParams& simParams,
                      utils::ParticlesData& data)
{
    constexpr int oneElement{1};
    comm.Bcast(&simParams.iterations, oneElement, MPI::UNSIGNED_LONG,
               masterNodeRank);
    comm.Bcast(&simParams.timeStep, oneElement, MPI::DOUBLE, masterNodeRank);
    comm.Bcast(&simParams.saveStep, oneElement, MPI::UNSIGNED, masterNodeRank);

    utils::MpiDatatypeRAII pointMpiType{utils::Point::mpiType()};

    comm.Bcast(data.positions.data(), config.totalParticles, pointMpiType,
               masterNodeRank);
    comm.Bcast(data.masses.data(), config.totalParticles, MPI::INT,
               masterNodeRank);

    const bool isMasterNode{config.nodeRank == masterNodeRank};
    comm.Scatterv(data.velocities.data(), config.particlesPerNode.data(),
                  config.offsetPerNode.data(), pointMpiType,
                  isMasterNode ? MPI::IN_PLACE : data.velocities.data(),
                  config.localParticles, pointMpiType, masterNodeRank);
};
} // namespace node::common

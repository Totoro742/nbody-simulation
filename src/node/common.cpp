#include "node/common.hpp"

#include "constants.hpp"
#include "node/NodeConfig.hpp"
#include <algorithm>
#include <cstdio>
#include <iterator>
#include <numeric>
#include <optional>
#include <upcxx/upcxx.hpp>

namespace
{
/*
 * calculate num of local particles
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
node::NodeConfig createNodeConfig(const std::optional<int> totalParticles)
{
    node::NodeConfig config{.totalNodes = config.totalNodes = upcxx::rank_n(),
                            .nodeRank = config.nodeRank = upcxx::rank_me(),
                            .totalParticles = totalParticles.value_or(0)};

    config.totalParticles =
        upcxx::broadcast(config.totalParticles, masterNodeRank).wait();

    calcParticlesPerNode(config);

    return config;
};

void initialShareData(const NodeConfig& config,
                      utils::SimParams& simParams,
                      utils::ParticlesData& data)
{
    simParams.iterations =
        upcxx::broadcast(simParams.iterations, masterNodeRank).wait();
    simParams.timeStep =
        upcxx::broadcast(simParams.timeStep, masterNodeRank).wait();
    simParams.saveStep =
        upcxx::broadcast(simParams.saveStep, masterNodeRank).wait();

    upcxx::broadcast(data.masses.data(), config.totalParticles, masterNodeRank)
        .wait();
};

void rputOverDistributed(const utils::PointVector& data,
                         const NodeConfig& config,
                         const DistData& distData)
{
    auto future = upcxx::make_future();
    for (int rank{0}; rank < config.totalNodes; ++rank) {
        auto callback{
            [&data, &config, rank](const upcxx::global_ptr<utils::Point> ptr) {
                return upcxx::rput(data.data() + config.offsetPerNode[rank],
                                   ptr, config.particlesPerNode[rank]);
            }};
        const auto new_future{distData.fetch(rank).then(callback)};
        future = upcxx::when_all(future, new_future);
    }
    future.wait();
}

void rgetOverDistributed(utils::PointVector& data,
                         const NodeConfig& config,
                         const DistData& distData)
{
    auto future = upcxx::make_future();
    for (int rank{0}; rank < config.totalNodes; ++rank) {
        const auto callback{[&data, &config,
                             rank](const upcxx::global_ptr<utils::Point> ptr) {
            return upcxx::rget(ptr, data.data() + config.offsetPerNode[rank],
                               config.particlesPerNode[rank]);
        }};
        const auto new_future{distData.fetch(rank).then(callback)};
        future = upcxx::when_all(future, new_future);
    }
    future.wait();
}
} // namespace node::common

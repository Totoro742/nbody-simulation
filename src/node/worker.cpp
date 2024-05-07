#include "node/worker.hpp"

#include "constants.hpp"
#include "node/NodeConfig.hpp"
#include "node/common.hpp"
#include "utils/ParticlesData.hpp"
#include "utils/Point.hpp"
#include "utils/SimParams.hpp"
#include <vector>

namespace
{
utils::ParticlesData createStoreForParticles(const node::NodeConfig& config)
{
    return {std::vector<utils::Point>(config.totalParticles),
            std::vector<utils::Point>(config.localParticles),
            std::vector<float>(config.totalParticles)};
}
} // namespace

namespace node::worker
{
void run(const MPI::Comm& comm)
{
    const auto config{common::createNodeConfig(comm)};

    if (config.totalParticles == 0) {
        return;
    }

    utils::SimParams simParams{};
    auto data{createStoreForParticles(config)};
    common::initialShareData(comm, config, simParams, data);

    const auto savePositionsOnMaster{
        [&](utils::PointVector& positions, const MPI::Datatype& pointMpiType) {
            const auto offset{config.offsetPerNode[config.nodeRank]};
            const auto localPositions{positions.begin() + offset};

            comm.Gatherv(localPositions->begin(), config.localParticles,
                         pointMpiType, nullptr, nullptr, nullptr,
                         MPI::DATATYPE_NULL, masterNodeRank);
        }};

    common::performAlgorithm(comm, config, simParams, data,
                             savePositionsOnMaster);
}
} // namespace node::worker

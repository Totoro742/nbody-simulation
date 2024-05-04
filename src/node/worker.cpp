#include "node/worker.hpp"

#include "node/NodeConfig.hpp"
#include "node/initial.hpp"
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
    const auto config{initial::createNodeConfig(comm)};

    if (config.totalParticles == 0) {
        return;
    }

    utils::SimParams simParams{};
    auto data{createStoreForParticles(config)};
    initial::shareData(comm, config, simParams, data);
}
} // namespace node::worker

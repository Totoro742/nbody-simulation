#include "node/worker.hpp"

#include "node/NodeConfig.hpp"
#include "node/common.hpp"
#include "utils/ParticlesData.hpp"
#include "utils/Point.hpp"
#include "utils/SimParams.hpp"
#include <cstdio>
#include <upcxx/upcxx.hpp>
#include <vector>

namespace
{
utils::ParticlesData createStoreForParticles(const node::NodeConfig& config)
{
    return {utils::PointVector(config.totalParticles),
            utils::PointVector(config.localParticles),
            std::vector<float>(config.totalParticles)};
}
} // namespace

namespace node::worker
{
void run()
{
    const auto config{common::createNodeConfig()};

    if (config.totalParticles == 0) {
        return;
    }

    utils::SimParams simParams{};
    auto data{createStoreForParticles(config)};
    common::initialShareData(config, simParams, data);
    const common::DistData distData{upcxx::dist_object{
        upcxx::new_array<utils::Point>(config.localParticles)}};
    upcxx::barrier();

    // distribute velocities
    upcxx::barrier();

    // copy on workers
    const auto offset{config.offsetPerNode[config.nodeRank]};
    const auto local_ptr{distData->local()};
    std::copy(local_ptr, local_ptr + config.localParticles,
              data.velocities.data());

    upcxx::barrier();

    // distribute positions
    upcxx::barrier();

    // copy on workers with offset
    std::copy(local_ptr, local_ptr + config.localParticles,
              data.positions.data() + offset);

    const auto emptyCallback{[&](utils::PointVector&, const int) {}};

    common::performAlgorithm(config, simParams, data, distData, emptyCallback);

    upcxx::delete_array(*distData);
}
} // namespace node::worker

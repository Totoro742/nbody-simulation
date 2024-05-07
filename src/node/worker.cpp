#include "node/worker.hpp"

#include "algorithms/Leapfrog7.hpp"
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

    // TODO looping
    algorithms::Leapfrog7 leapfrog{data, {0, config.localParticles}};

    auto pointMpiType{utils::Point::mpiType()};
    pointMpiType.Commit();

    const auto shareFunction{[&](std::vector<utils::Point>& positions) {
        comm.Allgatherv(MPI::IN_PLACE, 0, MPI::DATATYPE_NULL, positions.data(),
                        config.particlesPerNode.data(),
                        config.offsetPerNode.data(), pointMpiType);
    }};
    leapfrog.performStep(1, shareFunction);

    pointMpiType.Free();
}
} // namespace node::worker

#include "node/master.hpp"

#include "node/initial.hpp"
#include "utils/ParticlesData.hpp"
#include "utils/Point.hpp"
#include <mpi.h>

namespace node::master
{
void run(const MPI::Comm& comm, std::vector<std::string_view>& args)
{
    // TODO handle arguments
    // TODO load data

    constexpr int totalParticlesDummy{1000};
    utils::ParticlesData dataDummy{
        std::vector(totalParticlesDummy, utils::Point{0, 1, 2}),
        std::vector(totalParticlesDummy, utils::Point{5, 5, 5}),
        std::vector(totalParticlesDummy, 21.37f)};

    const auto config{initial::createNodeConfig(comm, totalParticlesDummy)};

    initial::shareData(comm, config, dataDummy);

    // TODO algorithm
}
} // namespace node::master

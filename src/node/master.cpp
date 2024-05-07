#include "node/master.hpp"

#include "algorithms/Leapfrog7.hpp"
#include "argparse/argparse.hpp"
#include "node/common.hpp"
#include "utils/DataLoader.hpp"
#include "utils/ParticlesData.hpp"
#include "utils/Point.hpp"
#include "utils/SimParams.hpp"
#include <cstdio>
#include <exception>
#include <mpi.h>
#include <optional>
#include <string>
#include <vector>

namespace
{
struct ProgramOptions {
    std::string fileInput;
    std::string fileOutput;
    utils::SimParams simParams;
};

std::optional<ProgramOptions>
parseArguments(const std::vector<std::string>& args)
{
    ProgramOptions options{};
    argparse::ArgumentParser parser{"nbody-mpi"};

    parser.add_argument("-t", "--time-step")
        .help("time step for calculations")
        .scan<'g', float>()
        .required()
        .store_into(options.simParams.timeStep);

    parser.add_argument("-n", "--iterations")
        .help("number of iterations")
        .scan<'u', unsigned long>()
        .required()
        .store_into(options.simParams.iterations);

    parser.add_argument("--input-file", "-i")
        .help("path to file with input data")
        .required()
        .store_into(options.fileInput);

    parser.add_argument("--output-file", "-o")
        .help("path to file where output data will be saved")
        .default_value("output.csv")
        .nargs(1)
        .store_into(options.fileOutput);

    try {
        parser.parse_args(args);
    } catch (const std::exception& err) {
        std::cerr << err.what() << std::endl;
        std::cerr << parser;
        return std::nullopt;
    }

    return options;
}
} // namespace

namespace node::master
{
void run(const MPI::Comm& comm, const std::vector<std::string>& args)
{
    const auto programOptions{parseArguments(args)};

    // stop processes by sending 0 as number of particles
    if (not programOptions.has_value()) {
        constexpr int zeroParticles{0};
        common::createNodeConfig(comm, zeroParticles);
        return;
    }

    utils::ParticlesData data{
        utils::loadParticlesData(programOptions->fileInput)};

    const auto totalParticles{data.positions.size()};
    const auto config{common::createNodeConfig(comm, totalParticles)};

    if (totalParticles == 0) {
        std::fprintf(stderr, "No particles in file %s\n",
                     programOptions->fileInput.c_str());
        return;
    }

    common::initialShareData(comm, config, programOptions->simParams, data);
    data.velocities.resize(config.localParticles);

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
} // namespace node::master

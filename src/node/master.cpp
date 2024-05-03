#include "node/master.hpp"

#include "argparse/argparse.hpp"
#include "node/initial.hpp"
#include "utils/ParticlesData.hpp"
#include "utils/Point.hpp"
#include "utils/SimParams.hpp"
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
    const auto options{parseArguments(args)};

    // stop processes by sending 0 as number of particles
    if (not options.has_value()) {
        constexpr int zeroParticles{0};
        initial::createNodeConfig(comm, zeroParticles);
        return;
    }

    // TODO load data

    constexpr int totalParticlesDummy{1000};
    utils::ParticlesData dataDummy{
        std::vector(totalParticlesDummy, utils::Point{0, 1, 2}),
        std::vector(totalParticlesDummy, utils::Point{5, 5, 5}),
        std::vector(totalParticlesDummy, 21.37f)};

    const auto config{initial::createNodeConfig(comm, totalParticlesDummy)};

    initial::shareData(comm, config, options->simParams, dataDummy);

    // TODO algorithm
}
} // namespace node::master

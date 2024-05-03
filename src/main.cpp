#include "constants.hpp"
#include "node/master.hpp"
#include "node/worker.hpp"
#include <cstdio>
#include <exception>
#include <mpi.h>
#include <string>
#include <vector>

int main(int argc, char** argv)
{
    MPI::Init(argc, argv);

    try {
        const int nodeRank{MPI::COMM_WORLD.Get_rank()};
        if (nodeRank == masterNodeRank) {
            const std::vector<std::string> args{argv, argv + argc};
            node::master::run(MPI::COMM_WORLD, args);
        } else {
            node::worker::run(MPI::COMM_WORLD);
        }
    } catch (const std::exception& err) {
        std::fprintf(stderr, "handling exception in main:\n  %s\n", err.what());
    } catch (...) {
        std::fprintf(stderr, "unknown exception\n");
    }

    MPI::Finalize();
    return 0;
}

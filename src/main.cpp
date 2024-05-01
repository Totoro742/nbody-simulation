#include "constants.hpp"
#include "node/master.hpp"
#include "node/worker.hpp"
#include <mpi.h>
#include <string_view>
#include <vector>

int main(int argc, char** argv)
{
    MPI::Init(argc, argv);

    const int nodeRank{MPI::COMM_WORLD.Get_rank()};
    if (nodeRank == masterNodeRank) {
        std::vector<std::string_view> args{argv, argv + argc};
        node::master::run(MPI::COMM_WORLD, args);
    } else {
        node::worker::run(MPI::COMM_WORLD);
    }

    MPI::Finalize();
    return 0;
}

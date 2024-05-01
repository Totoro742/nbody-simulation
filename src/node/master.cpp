#include "node/master.hpp"

#include "node/initial.hpp"
#include <mpi.h>

namespace node::master
{
void run(const MPI::Comm& comm, std::vector<std::string_view>& args)
{
    // TODO handle arguments
    // TODO load data

    constexpr int placeholder{1000};

    const auto config{node::createNodeConfig(comm, placeholder)};

    // TODO algorithm
}
} // namespace node::master

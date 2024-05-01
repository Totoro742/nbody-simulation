#include "node/worker.hpp"

#include "node/initial.hpp"

namespace node::worker
{
void run(const MPI::Comm& comm)
{
    const auto config{node::createNodeConfig(comm)};

    // TODO algorithm
}
} // namespace node::worker

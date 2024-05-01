#pragma once

#include <mpi.h>

namespace node::worker
{
void run(const MPI::Comm& comm);
}

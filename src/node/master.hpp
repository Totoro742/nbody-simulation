#pragma once

#include <mpi.h>
#include <string>
#include <vector>

namespace node::master
{
void run(const MPI::Comm& comm, const std::vector<std::string>& args);
}

#pragma once

#include <mpi.h>
#include <string_view>
#include <vector>

namespace node::master
{
void run(const MPI::Comm& comm, std::vector<std::string_view>& args);
}

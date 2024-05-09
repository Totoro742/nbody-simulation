#pragma once

#include <mpi.h>

namespace utils
{
class MpiDatatypeRAII
{
public:
    MpiDatatypeRAII(MPI::Datatype&& datatype) : datatype{datatype}
    {
        this->datatype.Commit();
    }

    ~MpiDatatypeRAII() { this->datatype.Free(); }

    operator MPI::Datatype&() { return datatype; }

private:
    MPI::Datatype datatype;
};
} // namespace utils

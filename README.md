# N-body simulation

Distributed program for collega course

## Building

Possible way to init MPI enviroment

```bash
eval "$(modulecmd bash load mpi/mpich-x86_64)"
```

Building

```bash
cmake -B build/ -DCMAKE_CXX_COMPILER=mpic++
cmake --build build
```

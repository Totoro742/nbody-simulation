# N-body simulation

Distributed program for college course

## Building

Possible way to init MPI environment

```bash
eval "$(modulecmd bash load mpi/mpich-x86_64)"
```

Building

```bash
cmake -B build/ -DCMAKE_CXX_COMPILER=mpic++
cmake --build build
```

## Execution

Example execution, see `--help` for more information

```bash
mpiexec -n 4 ./build/src/nbody-mpi -i data.csv -n 5000 -t 1
```

## Clean up

```bash
cmake --build build/ --target clean-all
```

## Example input and output

In directory `sample-data` two files are present:

- `sample-symmetric.csv` - sample input data to program
- `sample-symmetric-output.csv` - sample output data from program
executed with simulation controlling arguments:
    
    `--iterations 5000 --time-step 1 --save-step 50`

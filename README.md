# N-body simulation

Distributed program for college course

## Building

Possible way to init UPCXX environment (needed for run)

```bash
module load upcxx/2023.9.0
```

Building

```bash
cmake -B build/ -DCMAKE_PREFIX_PATH=<path-to-upcxx-install-dir>/share/cmake/
cmake --build build
```

## Execution

Example execution, see `--help` for more information

```bash
upcxx-run -n 4 -ssh-servers <HOSTS> -- ./build/src/nbody-upcxx -i data.csv -n 5000 -t 1
# or
upcxx-run -n 4 -localhost -- ./build/src/nbody-upcxx -i data.csv -n 5000 -t 1
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

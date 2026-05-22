# C Program Test With Slurm

This directory is set up so compilation can happen on the head node, while program execution happens only through Slurm.
The Slurm job scripts target the `fire` node.

## Compile on the head node

```sh
make
```

## Run through Slurm

```sh
make submit
```

Or submit the job script directly:

```sh
make
mkdir -p logs
sbatch run.sbatch
```

## Check status and output

```sh
squeue -u "$USER"
ls logs
cat logs/c-test-<jobid>.out
cat logs/c-test-<jobid>.err
```

Do not run `./hello` directly on the head node. Use `sbatch run.sbatch` or `make submit`.

## OpenMP matrix multiplication test

`matmul_omp.c` performs a simple dense matrix multiplication with the usual `i-j-k`
triple loop and parallelizes the outer `i` loop with OpenMP.

Compile on the head node:

```sh
make matmul_omp
```

Run through Slurm:

```sh
make submit-matmul
```

The Slurm script tests several thread counts by setting `OMP_NUM_THREADS`.
The defaults are:

```sh
MATRIX_SIZE=800
REPEATS=3
THREAD_LIST="1 2 4 8"
```

Override them at submission time when needed:

```sh
MATRIX_SIZE=1000 REPEATS=2 THREAD_LIST="1 2 4 8" sbatch matmul_openmp.sbatch
```

The output CSV columns are:

```text
run,time_sec,gflops,checksum
```

Do not run `./matmul_omp` directly on the head node. Use
`sbatch matmul_openmp.sbatch` or `make submit-matmul`.

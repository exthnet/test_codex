# C Program Test With Slurm

This directory is set up so compilation can happen on the head node, while program execution happens only through Slurm.

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

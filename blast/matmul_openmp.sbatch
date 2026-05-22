#!/usr/bin/env bash
#SBATCH --job-name=matmul-omp
#SBATCH --partition=fire
#SBATCH --nodelist=fire
#SBATCH --output=logs/%x-%j.out
#SBATCH --error=logs/%x-%j.err
#SBATCH --time=00:15:00
#SBATCH --ntasks=1
#SBATCH --cpus-per-task=8

set -euo pipefail

cd "$SLURM_SUBMIT_DIR"

matrix_size="${MATRIX_SIZE:-800}"
repeats="${REPEATS:-3}"
thread_list="${THREAD_LIST:-1 2 4 8}"

echo "SLURM_JOB_ID=${SLURM_JOB_ID}"
echo "SLURMD_NODENAME=${SLURMD_NODENAME:-unknown}"
echo "SLURM_CPUS_PER_TASK=${SLURM_CPUS_PER_TASK:-unset}"
echo "MATRIX_SIZE=${matrix_size}"
echo "REPEATS=${repeats}"
echo "THREAD_LIST=${thread_list}"
echo

for threads in ${thread_list}; do
    export OMP_NUM_THREADS="${threads}"
    export OMP_PLACES="${OMP_PLACES:-cores}"
    export OMP_PROC_BIND="${OMP_PROC_BIND:-close}"

    echo "== OMP_NUM_THREADS=${OMP_NUM_THREADS} =="
    ./matmul_omp "${matrix_size}" "${repeats}"
    echo
done

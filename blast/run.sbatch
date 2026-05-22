#!/usr/bin/env bash
#SBATCH --job-name=c-test
#SBATCH --partition=fire
#SBATCH --nodelist=fire
#SBATCH --output=logs/%x-%j.out
#SBATCH --error=logs/%x-%j.err
#SBATCH --time=00:05:00
#SBATCH --ntasks=1
#SBATCH --cpus-per-task=1

set -euo pipefail

cd "$SLURM_SUBMIT_DIR"
./hello

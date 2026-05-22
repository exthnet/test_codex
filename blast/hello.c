#include <stdio.h>
#include <stdlib.h>

int main(void) {
    const char *job_id = getenv("SLURM_JOB_ID");
    const char *node = getenv("SLURMD_NODENAME");

    printf("Hello from C via Slurm\n");
    printf("SLURM_JOB_ID=%s\n", job_id ? job_id : "(not set)");
    printf("SLURMD_NODENAME=%s\n", node ? node : "(not set)");

    return 0;
}

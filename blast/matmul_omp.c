#include <errno.h>
#include <limits.h>
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

static int parse_positive_int(const char *text, const char *name) {
    char *end = NULL;
    errno = 0;
    long value = strtol(text, &end, 10);

    if (errno != 0 || end == text || *end != '\0' || value <= 0 || value > INT_MAX) {
        fprintf(stderr, "invalid %s: %s\n", name, text);
        exit(2);
    }

    return (int)value;
}

static void *checked_malloc(size_t bytes) {
    void *ptr = malloc(bytes);
    if (ptr == NULL) {
        fprintf(stderr, "failed to allocate %zu bytes\n", bytes);
        exit(1);
    }
    return ptr;
}

static void init_matrices(double *a, double *b, double *c, int n) {
    #pragma omp parallel for schedule(static)
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            a[(size_t)i * n + j] = (double)((i + j) % 100) / 100.0;
            b[(size_t)i * n + j] = (double)((i * 3 + j * 7) % 100) / 100.0;
            c[(size_t)i * n + j] = 0.0;
        }
    }
}

static void matmul_omp(const double *a, const double *b, double *c, int n) {
    #pragma omp parallel for schedule(static)
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            double sum = 0.0;
            for (int k = 0; k < n; ++k) {
                sum += a[(size_t)i * n + k] * b[(size_t)k * n + j];
            }
            c[(size_t)i * n + j] = sum;
        }
    }
}

static double checksum(const double *c, int n) {
    double sum = 0.0;

    #pragma omp parallel for reduction(+:sum) schedule(static)
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            sum += c[(size_t)i * n + j];
        }
    }

    return sum;
}

int main(int argc, char **argv) {
    int n = 800;
    int repeats = 3;

    if (argc >= 2) {
        n = parse_positive_int(argv[1], "matrix size");
    }
    if (argc >= 3) {
        repeats = parse_positive_int(argv[2], "repeat count");
    }
    if (argc > 3) {
        fprintf(stderr, "usage: %s [matrix_size] [repeats]\n", argv[0]);
        return 2;
    }

    size_t elements = (size_t)n * (size_t)n;
    size_t bytes = elements * sizeof(double);
    double *a = checked_malloc(bytes);
    double *b = checked_malloc(bytes);
    double *c = checked_malloc(bytes);

    init_matrices(a, b, c, n);

    int threads = 0;
    #pragma omp parallel
    {
        #pragma omp single
        threads = omp_get_num_threads();
    }

    printf("matrix_size=%d repeats=%d threads=%d\n", n, repeats, threads);
    printf("implementation=ijk_outer_i_parallel schedule=static\n");
    printf("run,time_sec,gflops,checksum\n");

    for (int r = 1; r <= repeats; ++r) {
        double start = omp_get_wtime();
        matmul_omp(a, b, c, n);
        double elapsed = omp_get_wtime() - start;
        double ops = 2.0 * (double)n * (double)n * (double)n;
        double gflops = ops / elapsed / 1.0e9;
        double check = checksum(c, n);

        printf("%d,%.6f,%.6f,%.12e\n", r, elapsed, gflops, check);
    }

    free(a);
    free(b);
    free(c);
    return 0;
}

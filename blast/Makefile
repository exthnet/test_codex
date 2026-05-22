CC ?= gcc
CFLAGS ?= -O2 -Wall -Wextra -std=c11
OMPFLAGS ?= -fopenmp
HELLO_TARGET ?= hello
MATMUL_TARGET ?= matmul_omp

.PHONY: all clean submit submit-matmul logs

all: $(HELLO_TARGET) $(MATMUL_TARGET)

$(HELLO_TARGET): hello.c
	$(CC) $(CFLAGS) -o $@ $^

$(MATMUL_TARGET): matmul_omp.c
	$(CC) $(CFLAGS) $(OMPFLAGS) -o $@ $^

logs:
	mkdir -p logs

submit: $(HELLO_TARGET) logs
	sbatch run.sbatch

submit-matmul: $(MATMUL_TARGET) logs
	sbatch matmul_openmp.sbatch

clean:
	rm -f $(HELLO_TARGET) $(MATMUL_TARGET)
	rm -rf logs

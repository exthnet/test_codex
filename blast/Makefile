CC ?= gcc
CFLAGS ?= -O2 -Wall -Wextra -std=c11
TARGET ?= hello
SOURCES ?= hello.c

.PHONY: all clean submit logs

all: $(TARGET)

$(TARGET): $(SOURCES)
	$(CC) $(CFLAGS) -o $@ $^

logs:
	mkdir -p logs

submit: all logs
	sbatch run.sbatch

clean:
	rm -f $(TARGET)
	rm -rf logs

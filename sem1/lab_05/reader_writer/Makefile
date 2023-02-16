CC = gcc
INCLUDE_DIR = include
# INCLUDE_DIR = ../backup/3/include
BINARIES = $(patsubst src/%.c, bin/%.o, $(wildcard src/*.c))
# BINARIES = $(patsubst ../backup/3/src/%.c,../backup/3/bin/%.o, $(wildcard ../backup/2/src/*.c))
# Remove -g if not in debug mode
COMPILE_FLAGS = -Wall -Werror -std=c99

.PHONY: all test clean
all: app.out

run: app.out
	./app.out

# app.out:
# 	$(CC) ../backup/3/src/main.c ../backup/3/src/io_objects.c -o $@ -I $(INCLUDE_DIR)
app.out: $(BINARIES)
	$(CC) $^ -o $@ -I $(INCLUDE_DIR)

bin/%.o: src/%.c
	mkdir -p bin
	$(CC) -c -o $@ -I$(INCLUDE_DIR) $(COMPILE_FLAGS) $^

clean:
	rm -rf bin/ app.out

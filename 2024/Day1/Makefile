# Define the compiler and flags.
CC = clang
CFLAGS = -Wall -Wextra -O2 -std=c11 -g

# "all" target builds both executables.
all: part1 part2

# Build part1 from part1.c.
part1: part1.c
	$(CC) $(CFLAGS) -o part1 part1.c

# Build part2 from part2.c.
part2: part2.c
	$(CC) $(CFLAGS) -o part2 part2.c

# Run part1 with input.txt.
run1: part1
	./part1 < input.txt

# Run part2 with input.txt.
run2: part2
	./part2 < input.txt

# Clean up the build artifacts.
clean:
	rm -rf part1 part1.dSYM part2 part2.dSYM

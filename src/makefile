# Usage:
# make        # compile all binary
# make clean  # remove ALL binaries and objects

.PHONY = all clean

CC = g++			# compiler to use

main:
	${CC} main.cpp morph.cpp -o main -lSDL2 -lSDL2_image

clean:
	@echo "Cleaning up..."
	rm -rvf main
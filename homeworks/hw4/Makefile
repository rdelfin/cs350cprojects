CC=gcc
DEPS = bitops.h bitmatrix.h
OBJ = bitops.o bitmatrix.o main.o

%.o: %.c $(DEPS)
	$(CC) -c -o build/$@ $<

all: main.o bitmatrix.o bitops.o
	gcc -g -o bin/graph build/main.o build/bitmatrix.o build/bitops.o -I.

clean:
	rm -rf bin/* build/*

CC=g++

LIB_FLAGS=-lm -lpng

NVCC=nvcc
NVCC_FLAGS=$(LIB_FLAGS)

# this guy will vary from system to system
CUDA_INCLUDEPATH=/opt/cuda/include

CFLAGS=$(LIB_FLAGS) -Wall -I $(CUDA_INCLUDEPATH)
CC=gcc
CFLAGS=-Wall -std=c89 -MD

EXE=pngr
DEPS=pngen.h pngr.h
OBJS=pngen.o pngr.o

.PHONY: all
all: $(EXE)

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

$(EXE): $(OBJS)
	$(CC) -o $@ $^ $(NVCC_FLAGS)

.PHONY: clean
clean:
	rm -f $(EXE)
	rm -f *.o
	rm -f *.d
	rm -f *.png
	fm -f *~


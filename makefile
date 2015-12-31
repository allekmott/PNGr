CC=g++

LIB_FLAGS=-lm -lpng

NVCC=nvcc
NVCC_FLAGS=$(LIB_FLAGS)

# this guy will vary from system to system
CUDA_INCLUDEPATH=/opt/cuda/include

CFLAGS=$(LIB_FLATS) -Wall -I $(CUDA_INCLUDEPATH)

EXE=pngr

DEPS=pngr.h pngen.h

OBJ_DEPS=pngen.o pngen_cuda.o pngr.o 

#%.o: %.c $(DEPS)
#	$(CC) -c -o $@ $< $(CFLAGS)

$(EXE): $(OBJ_DEPS)
	$(NVCC) -o $@ $^ $(NVCC_FLAGS)

pngen.o: pngen.c pngen.h
	$(CC) -c $< $(CFLAGS)

pngr.o: pngr.c pngr.h pngen.h pngen_cuda.h
	$(CC) -c $< $(CFLAGS)

pngen_cuda.o: pngr.h pngen.h pngen_cuda.h
	$(NVCC) -c pngen_cuda.cu $(NVCC_FLAGS)

clean:
	rm -f $(EXE) *.o *.png *~

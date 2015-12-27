CC=gcc
CFLAGS=-lm -lpng

EXE=pngr

DEPS=pngr.h imgfapper.h

OBJ_DEPS=imgfapper.o pngr.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

$(EXE): $(OBJ_DEPS)
	$(CC) -o $@ $^ $(CFLAGS)

clean:
	rm -f $(EXE) *.o *.png *~

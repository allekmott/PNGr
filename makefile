CC=gcc
CFLAGS=-lm -lpng -I/usr/local/include -I/usr/local/lib

EXE=pngr

DEPS=pngr.h imgfapper.h

OBJ_DEPS=imgfapper.o pngr.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

$(EXE): $(OBJ_DEPS)
	$(CC) -o $@ $^ $(CFLAGS)

clean:
	rm -f $(EXE) *.o *.png *~

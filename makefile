CC=gcc
CFLAGS=

EXE=pngr

DEPS=pngr.h

OBJ_DEPS=pngr.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

$(EXE): $(OBJ_DEPS)
	$(CC) -o $@ $^ $(CFLAGS)

clean:
	rm -f $(EXE) *.o *.wav *~

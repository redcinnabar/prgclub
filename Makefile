CC=gcc
PROGRAM=graph-ex
CFLAGS=-O2 -std=c99 -Wall -pedantic `allegro-config --cflags`
LIBS=`allegro-config --libs`

$(PROGRAM): main.c
	$(CC) $(CFLAGS) $^ -o $@ $(LIBS)

clean:
	rm -f $(PROGRAM)

all: clean $(PROGRAM)

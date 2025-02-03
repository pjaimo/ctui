CFLAGS=-Wall -Wextra -pedantic -g
INCLUDES=-I.


main: main.c
	gcc $(CFLAGS) -o build/main main.c $(INCLUDES)

conway: examples/conway.c
	gcc $(CFLAGS) -o build/conway examples/conway.c $(INCLUDES)

tetris: examples/tetris.c
	gcc $(CFLAGS) -o build/tetris examples/tetris.c $(INCLUDES) -lm

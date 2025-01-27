CFLAGS=-Wall -Wextra -pedantic -g
INCLUDES=-I.


main: main.c
	gcc $(CFLAGS) -o build/main main.c

conway: examples/conway.c
	gcc $(CFLAGS) -o build/conway examples/conway.c $(INCLUDES)

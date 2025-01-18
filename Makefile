CFLAGS=-Wall -Wextra -pedantic

main: main.c
	gcc $(CFLAGS) -o main main.c
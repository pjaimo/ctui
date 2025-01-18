CFLAGS=-Wall -Wextra -pedantic -g

main: main.c
	gcc $(CFLAGS) -o build/main main.c
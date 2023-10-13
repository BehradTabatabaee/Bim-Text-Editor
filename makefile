all:
	gcc bim.c -o bim.o -Wall -Wextra -pedantic -std=c99
	./bim.o
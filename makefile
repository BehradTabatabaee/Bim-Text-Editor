all:
	gcc bim.c -o bim.o -Wall -Wextra -pedantic -std=c99
	./bim.o

clean:
	rm -rf ./bim.o
CC=gcc
CFLAGS=-Wall -Wextra -std=c11 -O2

all: main

main: main.o node.o list.o
	$(CC) $(CFLAGS) -o main main.o node.o list.o
main.o: main.c node.h list.h
	$(CC) $(CFLAGS) -c -o main.o main.c
node.o: node.c node.h list.h
	$(CC) $(CFLAGS) -c -o node.o node.c
list.o: list.c list.h node.h
	$(CC) $(CFLAGS) -c -o list.o list.c
clean:
	rm -f *.o
	rm -f main
.PHONY: all clean

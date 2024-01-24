CC = gcc
CFLAGS = -Wall -Wextra -g
INC = -I.

all: a.out

vector.o: vector.c vector.h
	$(CC) $(CFLAGS) -c vector.c $(INC) -o vector.o

heap.o: heap.c heap.h vector.h
	$(CC) $(CFLAGS) -c heap.c $(INC) -o heap.o

main.o: main.c heap.h vector.h
	$(CC) $(CFLAGS) -c main.c $(INC) -o main.o

a.out: main.o vector.o heap.o
	$(CC) $(CFLAGS) $^ -o a.out
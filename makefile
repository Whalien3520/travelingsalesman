# Makefile comments
PROGRAMS = main
CC = gcc
CFLAGS = -Wall -g
all: $(PROGRAMS)
clean:
	rm -f *.o
# C compilations
intArrayList.o: intArrayList.c intArrayList.h
	$(CC) $(CFLAGS) -c intArrayList.c
listArrayList.o: listArrayList.c listArrayList.h
	$(CC) $(CFLAGS) -c listArrayList.c
priorityQueue.o: priorityQueue.c priorityQueue.h
	$(CC) $(CFLAGS) -c priorityQueue.c
node.o: node.c node.h helper.h
	$(CC) $(CFLAGS) -c node.c
main.o: main.c node.h priorityQueue.h helper.h
	$(CC) $(CFLAGS) -c main.c
# Executable program
main: main.o node.o priorityQueue.o listArrayList.o intArrayList.o
	$(CC) $(CFLAGS) -o main main.o node.o priorityQueue.o listArrayList.o intArrayList.o

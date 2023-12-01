# Makefile comments
PROGRAMS = main
CC = gcc
CFLAGS = -Wall -g
all: $(PROGRAMS)
clean:
	rm -f *.o
# C compilations
helper.o: helper.c helper.h
	$(CC) $(CFLAGS) -c helper.c
intArrayList.o: intArrayList.c intArrayList.h helper.h
	$(CC) $(CFLAGS) -c intArrayList.c
listArrayList.o: listArrayList.c listArrayList.h intArrayList.h helper.h
	$(CC) $(CFLAGS) -c listArrayList.c
node.o: node.c node.h listArrayList.h listArrayList.h intArrayList.h
	$(CC) $(CFLAGS) -c node.c
priorityQueue.o: priorityQueue.c priorityQueue.h node.h
	$(CC) $(CFLAGS) -c priorityQueue.c
main.o: main.c node.h priorityQueue.h node.h helper.h
	$(CC) $(CFLAGS) -c main.c
# Executable program
main: main.o node.o priorityQueue.o listArrayList.o intArrayList.o helper.o
	$(CC) $(CFLAGS) -o main main.o node.o priorityQueue.o listArrayList.o intArrayList.o helper.o

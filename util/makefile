CC = gcc
CFLAGS = -Wall --std=c99 -g
OBJECTS = pqueue.o main.o

priority: main.o pqueue.o
	$(CC) -o priority $(OBJECTS)

main.o: main.c
	$(CC) $(CFLAGS) -c main.c -o main.o

pqueue.o: pqueue.c pqueue.h
	$(CC) $(CFLAGS) -c pqueue.c -o pqueue.o

clean:
	rm -f priority $(OBJECTS) *~

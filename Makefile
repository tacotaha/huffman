CC = gcc
CFLAGS = -Wall -g
OBJECTS = pqueue.o huffman.o main.o

huffman: $(OBJECTS) 
	$(CC) $(CFLAGS) -o huffman $(OBJECTS)

pqueue.o: utils/pqueue.c utils/pqueue.h
	$(CC) $(CFLAGS) -c utils/pqueue.c -o pqueue.o

huffman.o: huffman.c huffman.h
	$(CC) $(CFLAGS) -c huffman.c -o huffman.o

main.o: main.c
	$(CC) $(CFLAGS) -c main.c -o main.o

clean:
	rm -f huffman $(OBJECTS) *~
CC = gcc
CFLAGS = -Wall -Werror -g
OBJECTS = bitio.o pqueue.o huffman.o main.o

huffman: $(OBJECTS) 
	$(CC) $(CFLAGS) -o huffman $(OBJECTS)

bitio.o: utils/bitio.c utils/bitio.h
	$(CC) $(CFLAGS) -c utils/bitio.c -o bitio.o

pqueue.o: utils/pqueue.c utils/pqueue.h
	$(CC) $(CFLAGS) -c utils/pqueue.c -o pqueue.o

huffman.o: huffman.c huffman.h
	$(CC) $(CFLAGS) -c huffman.c -o huffman.o

main.o: main.c
	$(CC) $(CFLAGS) -c main.c -o main.o

clean:
	rm -f huffman $(OBJECTS) *~

format:
	find *.c *.h utils/*.c utils/*.h | xargs indent -par -br -brf -brs -kr -ci2 -cli2 -i2 -l80 -nut

CC=gcc
CFLAGS=-lpthread

default: all

server: server.o common.o vector.o
	$(CC) -o server server.o common.o vector.o $(CFLAGS)
	rm -f *.o

client: client.o common.o
	$(CC) -o client client.o common.o
	rm -f *.o

server.o: server.c common.h server.h
	$(CC) -g -c server.c

client.o: client.c common.h
	$(CC) -c client.c

common.o: common.c common.h
	$(CC) -c common.c

vector.o: vector.c vector.h
	$(CC) -g -c vector.c

all: server client

clean:
	rm -f *.o server client
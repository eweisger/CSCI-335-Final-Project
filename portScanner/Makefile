CC = gcc
CFLAGS = -o -Wall
LFLAGS = -pthread

all: portScanner

portScanner: main.c linkedList.c linkedList.h scanner.h scanner.c myThreads.h myThreads.c checks.h checks.c
	$(CC) $(CFLAGS) -c -o main.o main.c
	$(CC) $(CFLAGS) -c -o linkedList.o linkedList.c
	$(CC) $(CFLAGS) -c -o scanner.o scanner.c
	$(CC) $(CFLAGS) -c -o myThreads.o myThreads.c
	$(CC) $(CFLAGS) -c -o checks.o checks.c
	$(CC) $(LFLAGS) -o portScanner main.o linkedList.o scanner.o myThreads.o checks.o

clean:
	rm -f portScanner main.o linkedList.o scanner.o myThreads.o checks.o

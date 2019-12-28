all: part1 part2 server
CC = gcc
CFLAGS = -std=c11

part1: part1.c
	$(CC) -lpthread -g $(CFLAGS) part1.c -o part1

part2: part2-2.c
	$(CC) -lpthread -g $(CFLAGS) part2-2.c -o part2

server: quacker.c
	$(CC) -lpthread -g $(CFLAGS) quacker.c -o server

clean: 
	$(RM) *.exe

CC = gcc
CFLAGS = -Wall -pedantic

pa01: *.c
	$(CC) $(CFLAGS) *.c -o pa01


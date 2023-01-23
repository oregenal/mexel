CC=gcc
CFLAGS=-Wall -Wextra -std=c11 -pedantic -ggdb

mexel:main.c
	$(CC) $(CFLAGS) -o $@ $<

CC=gcc
CFLAGS=-Wall -Wextra -std=c11 -pedantic -ggdb
BIN=mexel
SRCS=main.c

$(BIN):$(SRCS)
	$(CC) $(CFLAGS) -o $@ $<

.PHONY:clean

clean:
	rm -rf $(BIN)

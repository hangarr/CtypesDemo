# Makefile to build a position independent library for the ctypesdemo C-code

.PHONY: all clean install

CFLAGS += -fPIC -O3 -std=c99 -DCPART

libctypesdemo.so: ctypesdemo.o
	$(CC) -shared -o $@ ctypesdemo.o

ctypesdemo.o : ctypesdemo.c
	$(CC) -Wall $(CFLAGS) -c $< -o $@

all: libctypesdemo.so

clean:
	rm -fr libctypesdemo.so

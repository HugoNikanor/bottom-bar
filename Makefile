.PHONY: all clean

# `make CC=winegcc` to build for windows on linux
CC=gcc
CFLAGS=-Wall -fPIC
LFLAGS=-lm

%.o: %.c %.h
	${CC} $(LFLAGS) -c -o $@ $<

%: %.o
	${CC} -o $@ $<

bar : hsvrgb.o main.o

clean:
	-rm *.o

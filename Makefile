.PHONY: all clean test

CC=gcc
CFLAGS=-Wall -fPIC -pthread -pedantic -I. -I../libtetris/src -ggdb
LIBS=-L../libtetris/ -ltetris -lm
C_FILES := $(wildcard src/*.c)
O_FILES := $(addprefix obj/,$(notdir $(C_FILES:.c=.o)))

obj/%.o: src/%.c
	$(CC) $(CFLAGS) -c -o $@ $<

bar : $(O_FILES)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

all : bar

# TODO figure out how to auto compile test files
# TODO Also figure out how to put shader files in own directory

clean:
	-rm obj/*.o
	-rm test/*.o

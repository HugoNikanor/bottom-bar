.PHONY: all clean

CC=gcc
CFLAGS=-Wall -fPIC -I.
LIBS=-lm
OBJ = hsvrgb.o main.o battery.o term_info.o util.o

%.o: %.c
	${CC} ${CFLAGS} -c -o $@ $<

%: %.o
	#${CC} ${CFLAGS} -o $@ $<
	${CC} ${CFLAGS} -o $@ $^

bar : $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

test : hsvrgb.o hsv-to-rgb-test.o
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

clean:
	-rm *.o

.PHONY: all clean

CC=gcc
CFLAGS=-Wall -fPIC -I.
LIBS=-lm
OBJ = hsvrgb.o main.o battery.o term_info.o util.o bad_shaders.o battery_shader.o

%.o: %.c
	${CC} ${CFLAGS} -c -o $@ $<

%: %.o
	#${CC} ${CFLAGS} -o $@ $<
	${CC} ${CFLAGS} -o $@ $^

bar : $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

test : hsvrgb.o hsv_to_rgb_test.o
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

clean:
	-rm *.o

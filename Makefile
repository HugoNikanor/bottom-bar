.PHONY: all clean test install_user

CC=gcc
CFLAGS=-Wall -fPIC -pthread -pedantic -I. -Ilib/libtetris/src -ggdb
LIBS=-Llib/libtetris -ltetris -lm
C_FILES := $(wildcard src/*.c)
O_FILES := $(addprefix obj/,$(notdir $(C_FILES:.c=.o)))
LIB_FILES := lib/libtetris/libtetris.a

all : bar

obj/%.o: src/%.c
	$(CC) $(CFLAGS) -c -o $@ $<

lib/libtetris/libtetris.a :
	make -C lib/libtetris/ libtetris.a

bar : $(O_FILES) $(LIB_FILES)
	$(CC) $(CFLAGS) $(LIBS) -o $@ $^ 

install_user: bar
	cp systemd/bar.service $$HOME/.config/systemd/user/bar.service
	systemctl enable --user bar
	systemctl start --user bar

install: bar
	cp systemd/bar.service /etc/systemd/system/bottom-bar.service
	cp bar /usr/bin/bottom-bar
	systemctl enable bottom-bar
	systemctl start bottom-bar

# TODO figure out how to auto compile test files
# TODO Also figure out how to put shader files in own directory

clean:
	-rm obj/*.o
	-rm test/*.o
	-make -C lib/libtetris/ clean

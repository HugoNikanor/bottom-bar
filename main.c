#include <stdio.h>
#include <stdlib.h>

// includes sleep
#include <unistd.h>

//include <linux/fb.h>
//include <fcntl.h>

#define LINES 66
#define HEIGHT 800
#define WIDTH 1280
#define FONT_HEIGHT 12

#define byte unsigned char

#define B 0
#define G 1
#define R 2
#define C G

/*
 * Programs which display stuff on the pixels left over in a tty
 * after the font has filled up as much as it cans.
 *
 * Currently only flashes different colours, but I want to have
 * some form of cool animation down there.
 *
 * Possibly some stick figures and a clock!
 */

int main() {
	srandom(1);

	int usable_lines = HEIGHT - FONT_HEIGHT * LINES;
	int data_size = usable_lines * WIDTH * 4;
	byte* data = malloc(data_size * sizeof(byte));

	FILE* f = fopen("/dev/fb0", "wb");
	for (int loop = 0;; loop++) {
		for (int i = 0; i < data_size; i += 4) {
			switch (loop % 3) {
				case 0: // BLUE
					data[i] = 0xFF;
					data[i+1] = 0;
					data[i+2] = 0;
					break;
				case 1: // Yellow
					data[i] = 0;
					data[i+1] = 0xFF;
					data[i+2] = 0xFF;
					break;
				case 2:
					data[i]   = 0xFF;
					data[i+1] = 0;
					data[i+2] = 0xFF;
			}
		}

		fseek(f, (HEIGHT * WIDTH * 4) - data_size, SEEK_SET);
		/* int read = */
		fwrite(data, sizeof(byte), data_size, f);
		//printf("%i, %i\n", data_size, read);
		int sleep_time = random() % 5;
		sleep(sleep_time);
	}
	fclose(f);
}

#include <stdio.h>
#include <stdlib.h>

// includes sleep
#include <unistd.h>

#define byte unsigned char

#define B 0
#define G 1
#define R 2

#define seconds * 1e6

/*
 * Programs which display stuff on the pixels left over in a tty
 * after the font has filled up as much as it cans.
 *
 * Currently only flashes different colours, but I want to have
 * some form of cool animation down there.
 *
 * Possibly some stick figures and a clock!
 */

// hsv -> rgb
// Hue Saturation Value

void setColor(
		byte* pixel,
		unsigned int x,
		unsigned int y,
		unsigned int loop)
{
	if (x % 0x200 > 0xFF && x % 0x200 < 0x200) {
		pixel[R] = -x;
		pixel[G] = -x;
	} else {
		pixel[R] = x;
		pixel[G] = x;
	}
}

int main() {
	srandom(1);
	unsigned static int LINES = 66;
	unsigned static int HEIGHT = 800;
	unsigned static int WIDTH = 1280;
	unsigned static int FONT_HEIGHT = 12;

	unsigned int usable_lines = HEIGHT - FONT_HEIGHT * LINES;
	unsigned int data_size = usable_lines * WIDTH * 4;
	byte* data = malloc(data_size * sizeof(byte));

	FILE* f = fopen("/dev/fb0", "wb");

	printf("WIDTH: %i\tusable_lines: %i\tdata_size: %i\n", WIDTH, usable_lines, data_size);

	for (unsigned int loop = 0; ; loop++) {
		for (unsigned int x = 0; x < WIDTH; x++) {
			for (unsigned int y = 0; y < usable_lines; y += 1) {
				unsigned int i = (y * WIDTH + x) * 4;
				setColor(data + i, x, y, loop);
			}
		}


		fseek(f, (HEIGHT * WIDTH * 4) - data_size, SEEK_SET);
		/* int read = */
		fwrite(data, sizeof(byte), data_size, f);
		//printf("%i, %i\n", data_size, read);
		usleep (0.1 seconds);
	}
	fclose(f);
}

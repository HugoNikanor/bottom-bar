#include <stdio.h>
#include <stdlib.h>

// includes sleep
#include <unistd.h>

#include "hsvrgb.h"

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

unsigned static int LINES;
unsigned static int HEIGHT;
unsigned static int WIDTH;
unsigned static int FONT_HEIGHT;
unsigned static int USABLE_LINES;
unsigned static int DATA_SIZE;

// hsv -> rgb
// Hue Saturation Value

void repeatingGradient(
		byte* pixel,
		unsigned int x,
		unsigned int y,
		unsigned long loop)
{
	pixel [B] = 0;
	if (x % 0x200 > 0xFF && x % 0x200 < 0x200) {
		pixel [R] = -x;
		pixel [G] = -x;
	} else {
		pixel [R] = x;
		pixel [G] = x;
	}
}

void oscilatingGradient(
		byte* pixel,
		unsigned int x,
		unsigned int y,
		unsigned long loop)
{

	int mloop;
	if (loop % (2 * WIDTH) < WIDTH) {
		mloop = loop % WIDTH;
	} else {
		mloop = WIDTH - loop % WIDTH;
	}

	pixel [R] = 0;
	if (x > mloop) {
		pixel [G] = 0;
		pixel [B] = 0;
	} else {
		double step = 0xFF / (double) WIDTH;
		pixel [G] = step * x;
		pixel [B] = step * x;
	}
}

void hsvGradient(
		byte* pixel,
		unsigned int x,
		unsigned int y,
		unsigned long loop)
{
	// H :: 0 -> 1/3
	// Goes from red to green via yellow
	double h = (1.0/3) * ((double) x/WIDTH);

	struct HSV hsv;
	hsv.h = h; hsv.s = 1; hsv.v = 1;

	struct RGB* rgb = hsv_to_rgb(&hsv);
	pixel [R] = rgb->r;
	pixel [G] = rgb->g;
	pixel [B] = rgb->b;
}

int main() {
	srandom(1);

	LINES        = 66;
	HEIGHT       = 800;
	WIDTH        = 1280;
	FONT_HEIGHT  = 12;
	USABLE_LINES = HEIGHT - FONT_HEIGHT * LINES;
	DATA_SIZE    = USABLE_LINES * WIDTH * 4;

	byte* data = malloc(DATA_SIZE * sizeof(byte));
	FILE* fb = fopen("/dev/fb0", "wb");
	void (*drawFunc)(byte*, int, int, long);

	printf("WIDTH: %i\tusable_lines: %i\tdata_size: %i\n", WIDTH, USABLE_LINES, DATA_SIZE);

	drawFunc = (void*) hsvGradient;
	for (unsigned long loop = 0; ; loop++) {
		// print loop counter
		if (loop % 100 == 0)
			printf("%li\n", loop);

		// run shader
		for (unsigned int x = 0; x < WIDTH; x++) {
			for (unsigned int y = 0; y < USABLE_LINES; y += 1) {
				unsigned int i = (y * WIDTH + x) * 4;
				drawFunc(data + i, x, y, loop);
			}
		}

		// transfer shader to screen
		fseek(fb, (HEIGHT * WIDTH * 4) - DATA_SIZE, SEEK_SET);
		/* int read = */
		fwrite(data, sizeof(byte), DATA_SIZE, fb);
		//printf("%i, %i\n", DATA_SIZE, read);

		// sleep
		usleep (0.004 seconds);
	}
	fclose(fb);
}

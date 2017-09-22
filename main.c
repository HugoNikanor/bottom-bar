#include <stdio.h>
#include <stdlib.h>

// includes sleep
#include <unistd.h>
// open
#include <fcntl.h>
// ioctl
#include <stropts.h>
#include <linux/fb.h>

#include "hsvrgb.h"
#include "battery.h"

typedef unsigned char byte;
//typedef u_int8_t byte;

#define B 0
#define G 1
#define R 2

#define seconds * 1e6

batteryData batData;

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

/*
 * Would be cool if this moved right if charing,
 * and left if decharching
 *
 * loop, x, y :: passthrough from the shader
 * speed :: how many pixels it should move per tick
 * width :: width of glider (edge to edge)
 * minimum :: default colour
 */
double gliderValueHelper(long int loop, int x, int y, int speed, int width, double minimum) {
	(void) y;


	int sideWidth = width / 2;
	// peak of glider, weird stuff is to enable it to be "offscreen"
	int highPos;
	if (batData.status == CHARGING) {
		highPos = ((loop * speed) % (WIDTH + width)) - sideWidth;
	} else { // if (batData.status == DISCHARGING) {
		highPos = WIDTH - ((loop * speed) % (WIDTH + width)) - sideWidth;
	}


	double diff = abs(highPos - x);

	if ( diff <= sideWidth ) {
		// higher value closer to the highPos
		double dwidth = (double) sideWidth;
		return minimum + ((dwidth - diff) / dwidth) * (1 - minimum);
	} else {
		return minimum;
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

	struct HSV hsv;
	hsv.h = (1.0/3) * ((double) x/WIDTH);
	hsv.s = 1;
	hsv.v = gliderValueHelper(loop, x, y, 10, 70, 0.6);

	struct RGB* rgb = hsv_to_rgb(&hsv);
	pixel [R] = rgb->r;
	pixel [G] = rgb->g;
	pixel [B] = rgb->b;
	free(rgb);
}

/*
 * Calls hsvGradient, but replaces with black if "outside"
 * battery range.
 */
void batteryShader(
		byte* pixel,
		unsigned int x,
		unsigned int y,
		unsigned long loop)
{
	hsvGradient(pixel, x, y, loop);
	if (x > batData.rate * WIDTH) {
		pixel [R] = 0;
		pixel [G] = 0;
		pixel [B] = 0;
	}
}

int main() {
	srandom(1);

	// Some information about how to get the terminal size
	// I'm however not sure what will be reported when started
	// through systemd.
	//
	// environment $TMUX to check how to get tty name
	// system("tty") => /dev/tty1 || /dev/pts/0
	// system("tmux display-message -p '#{client_tty}') => /dev/tty1
	// system("showconsolefont -iC /dev/tty1") => 8x12x256

	//FILE* fb = fopen("/dev/fb0", "wb");
	int fb_fd = open("/dev/fb0", O_RDWR);
	FILE* fb = fdopen(fb_fd, "wb");
	struct fb_var_screeninfo vinfo;
	ioctl(fb_fd, FBIOGET_VSCREENINFO, &vinfo);

	// HOW DO I GET THE FONT SIZE!
	// OR THE NUMBER OF LINES ON THE SCREEN!
	LINES        = 66; // HEIGHT / FONT_HEIGHT
	HEIGHT       = vinfo.yres; // 800
	WIDTH        = vinfo.xres; // 1280
	FONT_HEIGHT  = 12; // HEIGHT / LINES;

	USABLE_LINES = HEIGHT - FONT_HEIGHT * LINES;
	DATA_SIZE    = USABLE_LINES * WIDTH * 4;

	byte* data = malloc(DATA_SIZE * sizeof(byte));
	void (*drawFunc)(byte*, int, int, long);

	printf("WIDTH: %i\tusable_lines: %i\tdata_size: %i\n", WIDTH, USABLE_LINES, DATA_SIZE);

	drawFunc = (void*) batteryShader;
	for (unsigned long loop = 0; ; loop++) {
		// print loop counter
		if (loop % 100 == 0)
			printf("%li\n", loop);

		batData.rate = get_battery();
		batData.status = get_charge_status();

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
		//usleep (0.004 seconds);
		usleep (0.04 seconds); // 25 FPS
		//usleep (1 seconds);
	}
	fclose(fb);
}

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

// includes sleep
#include <unistd.h>
// open
#include <fcntl.h>
// ioctl
#include <sys/ioctl.h>
#include <linux/fb.h>

#include "battery.h"
#include "term_info.h"

#include "common.h"

#include "bad_shaders.h"
#include "battery_shader.h"
#include "tetris_shader.h"

/*
 * Programs which display stuff on the pixels left over in a tty
 * after the font has filled up as much as it cans.
 *
 * Currently only flashes different colours, but I want to have
 * some form of cool animation down there.
 *
 * Possibly some stick figures and a clock!
 */

// These are effectively const
unsigned int LINES;
unsigned int HEIGHT;
unsigned int WIDTH;
unsigned int FONT_HEIGHT;
unsigned int USABLE_LINES;
unsigned int DATA_SIZE;

batteryData batData;

#define USE_FONTINFO 1

int main() {
	srandom(1);

	//FILE* fb = fopen("/dev/fb0", "wb");
	int fb_fd = open("/dev/fb0", O_RDWR);
	if (fb_fd == -1) {
		printf("Opening framebuffer failed (%i) %s\n",
				errno, strerror(errno));
		return errno;
	}
	FILE* fb = fdopen(fb_fd, "wb");
	struct fb_var_screeninfo vinfo;
	ioctl(fb_fd, FBIOGET_VSCREENINFO, &vinfo);

	// While this work when running from a terminal,
	// it doesn't work when running from systemd.
	// TODO log some stuff somewhere, so I can actually
	//      see what happens.
#if USE_FONTINFO
	char* ttyname = get_tty();
	fontinfo fontinfo;
	get_font_info(ttyname, &fontinfo);
	free(ttyname);
#endif

	//printf("%i %i %i\n", fontinfo.height, fontinfo.width, fontinfo.chars);

	HEIGHT       = vinfo.yres; // 800
	WIDTH        = vinfo.xres; // 1280
	FONT_HEIGHT  =
#if USE_FONTINFO
		fontinfo.height; // 12 // HEIGHT / LINES;
#else
		16;
#endif
	LINES        = HEIGHT / FONT_HEIGHT; // 66

	USABLE_LINES = HEIGHT - FONT_HEIGHT * LINES;
	DATA_SIZE    = USABLE_LINES * WIDTH * 4;

	byte (*data)[WIDTH][4] = (byte (*)[WIDTH][4]) calloc(DATA_SIZE, sizeof(byte));
	void (*drawFunc)(byte[4], uint, uint, ulong);

	printf("Monitor size = %ix%i\n", WIDTH, HEIGHT);
	printf("Font height = %i\n", FONT_HEIGHT);
	printf("usable lines = %i\n", USABLE_LINES);

	shader sh;

	// this should possibly be set in config,
	// or something
	create_battery_shader (&sh);

	sh.init();
	drawFunc = sh.shader;

	for (unsigned long loop = 0; ; loop++) {
		// print loop counter
		/*
		if (loop % 100 == 0)
			printf("%li\n", loop);
			*/

		batData.rate = get_battery();
		batData.status = get_charge_status();

		// run shader
		for (unsigned int x = 0; x < WIDTH; x++) {
			for (unsigned int y = 0; y < USABLE_LINES; y += 1) {
				drawFunc(data[y][x], x, y, loop);
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

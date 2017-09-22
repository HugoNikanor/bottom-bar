#include <stdio.h>
#include <stdlib.h>

// includes sleep
#include <unistd.h>
// open
#include <fcntl.h>
// ioctl
#include <stropts.h>
#include <linux/fb.h>

#include "battery.h"
#include "term_info.h"

#include "common.h"

#include "bad_shaders.h"
#include "battery_shader.h"

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

int main() {
	srandom(1);

	//FILE* fb = fopen("/dev/fb0", "wb");
	int fb_fd = open("/dev/fb0", O_RDWR);
	FILE* fb = fdopen(fb_fd, "wb");
	struct fb_var_screeninfo vinfo;
	ioctl(fb_fd, FBIOGET_VSCREENINFO, &vinfo);

	// While this work when running from a terminal,
	// it doesn't work when running from systemd.
	// TODO log some stuff somewhere, so I can actually
	//      see what happens.
	/*
	char* ttyname = get_tty();
	fontinfo fontinfo;
	get_font_info(ttyname, &fontinfo);
	free(ttyname);
	*/

	//printf("%i %i %i\n", fontinfo.height, fontinfo.width, fontinfo.chars);

	HEIGHT       = vinfo.yres; // 800
	WIDTH        = vinfo.xres; // 1280
	FONT_HEIGHT  = 12; // fontinfo.height; // 12 // HEIGHT / LINES;
	LINES        = HEIGHT / FONT_HEIGHT; // 66

	USABLE_LINES = HEIGHT - FONT_HEIGHT * LINES;
	DATA_SIZE    = USABLE_LINES * WIDTH * 4;

	byte* data = malloc(DATA_SIZE * sizeof(byte));
	void (*drawFunc)(byte*, uint, uint, ulong);

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

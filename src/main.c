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


batteryData batData;

#define USE_FONTINFO 1

int main() {

	srandom(1);

	struct fb_var_screeninfo vinfo;
	FILE *fb;
	{
		int fb_fd = open("/dev/fb0", O_RDWR);
		if (fb_fd == -1) {
			printf("Opening framebuffer failed (%i) %s\n",
					errno, strerror(errno));
			return errno;
		}
		fb = fdopen(fb_fd, "wb");
		ioctl(fb_fd, FBIOGET_VSCREENINFO, &vinfo);
	}

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

	unsigned int screen_height = vinfo.yres; // 800
	unsigned int screen_width  = vinfo.xres; // 1280
	unsigned int font_height  =
#if USE_FONTINFO
		fontinfo.height; // 12
#else
		16;
#endif
	unsigned int text_lines = screen_height / font_height; // 66
	unsigned int usable_lines = screen_height - font_height * text_lines;

	unsigned int screen_size = screen_width * screen_height * 4;
	unsigned int usable_size = screen_width * usable_lines * 4;

	byte (*data)[screen_width][4] = (byte (*)[screen_width][4]) calloc(usable_size, sizeof(byte));

	printf("Monitor size = %ix%i\n", screen_width, screen_height);
	printf("Font height = %i\n", font_height);
	printf("usable lines = %i\n", usable_lines);


	shader *sh;
	// this should possibly be set in config, or something
#if 1
	sh = create_battery_shader ();
#else
	sh = create_tetris_shader();
#endif

	sh->init(sh, screen_width, usable_lines);

	for (unsigned long loop = 0; ; loop++) {
		// print loop counter
		/*
		if (loop % 100 == 0)
			printf("%li\n", loop);
			*/

		batData.rate = get_battery();
		batData.status = get_charge_status();

		// run shader
		for (unsigned int x = 0; x < screen_width; x++) {
			for (unsigned int y = 0; y < usable_lines; y += 1) {
				(sh->shader)(sh, data[y][x], x, y, loop);
			}
		}

		// transfer shader to screen
		fseek(fb, screen_size - usable_size, SEEK_SET);
		/* int read = */
		fwrite(data, sizeof(byte), usable_size, fb);
		//printf("%i, %i\n", DATA_SIZE, read);

		// sleep
		//usleep (0.004 seconds);
		usleep (0.04 seconds); // 25 FPS
		//usleep (1 seconds);
	}
	sh->cleanup(sh);
	free(data);
	fclose(fb);
}

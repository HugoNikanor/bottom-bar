// for abs
#include <stdlib.h>

#include "battery.h"
#include "hsvrgb.h"

#include "battery_shader.h"

/*
 * Would be cool if this moved right if charing,
 * and left if decharching
 *
 * intentionally not exported
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

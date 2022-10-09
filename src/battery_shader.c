// for abs
#include <stdlib.h>

#include "battery.h"
#include "hsvrgb.h"

#include "battery_shader.h"

//RGB* high_colors[WIDTH];
//RGB* low_colors [WIDTH];

// List of RGB pointers
RGB** high_colors;
RGB** low_colors ;

static int getHighPos(int, int, int);

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
static double gliderValueHelper(
		const ulong loop,
		const uint x,
		const uint y,
		const int speed,
		const int width,
		const double minimum,
		int highPos)
{
	(void) y;

	// how far the glider is visible from the center
	int sideWidth = width / 2;

	// distance to center
	double diff = abs(highPos - x);

	// check if within glider
	if ( diff <= sideWidth ) {
		// higher value closer to the highPos

		double dwidth = (double) sideWidth;
		return minimum + ((dwidth - diff) / dwidth) * (1 - minimum);
	} else {
		return -1;
	}
}

/*
 * Calculate where the senter of the glider should
 * be for the current tick.
 *
 * Note that the glider always travels the entire
 * length of the screen, even if that part is blacked
 * out.
 *
 * glider widith and glide speed should possibly
 * be set as semi global values instead.
 */
static int getHighPos(int loop, int speed, int glider_width) {
	int sideWidth = glider_width / 2;

	int highPos;
	if (batData.status == CHARGING) {
		highPos = ((loop * speed) % (WIDTH + glider_width)) - sideWidth;
	} else { // if (batData.status == DISCHARGING) {
		highPos = WIDTH - ((loop * speed) % (WIDTH + glider_width)) - sideWidth;
	}
	return highPos;
}

/*
 * TODO clean up obejects
 */
void init_shader () {

	//RGB* high_colors[WIDTH];
	//RGB* low_colors [WIDTH];

	// List of RGB pointers
	high_colors = malloc(sizeof(RGB*) * WIDTH);
	low_colors  = malloc(sizeof(RGB*) * WIDTH);

	HSV hsv;

	for (int x = 0; x < WIDTH; x++) {
		hsv.h = (1.0/3) * ((double) x/WIDTH);
		hsv.s = 1;
		hsv.v = 0.6;

		RGB* rgb1 = malloc(sizeof(*rgb1));
		hsv_to_rgb (&hsv, rgb1);
		low_colors [x] = rgb1;

		RGB* rgb2 = malloc(sizeof(*rgb2));
		hsv.v = 1;
		hsv_to_rgb (&hsv, rgb2);
		high_colors [x] = rgb2;
	}
}

void hsvGradient(
		byte pixel[4],
		const uint x,
		const uint y,
		const ulong loop)
{
	// H :: 0 -> 1/3
	// Goes from red to green via yellow

	int highPos = getHighPos (loop, 10, 70);

	RGB rgb;

	double vval;
	if (batData.status == FULL) {
		rgb = *high_colors [x];
	} else if ( (vval = gliderValueHelper(loop, x, y, 10, 70, 0.7, highPos)) != -1 ) {
		HSV hsv;
		hsv.h = (1.0/3) * ((double) x/WIDTH);
		hsv.s = 1;
		hsv.v = gliderValueHelper(loop, x, y, 10, 70, 0.6, highPos);
		hsv_to_rgb(&hsv, &rgb);
	} else {
		rgb = *low_colors [x];
	}

	pixel [R] = rgb.r;
	pixel [G] = rgb.g;
	pixel [B] = rgb.b;
}

/*
 * Calls hsvGradient, but replaces with black if "outside"
 * battery range.
 */
void batteryShader(
		byte pixel[4],
		const uint x,
		const uint y,
		const ulong loop)
{
	hsvGradient(pixel, x, y, loop);
	if (x > batData.rate * WIDTH) {
		pixel [R] = 0;
		pixel [G] = 0;
		pixel [B] = 0;
	}
}

void create_battery_shader (shader* sh) {
	sh->init = init_shader;
	sh->shader = batteryShader;
}


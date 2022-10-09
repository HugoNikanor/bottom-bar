// for abs
#include <stdlib.h>

#include "battery.h"
#include "hsvrgb.h"

#include "battery_shader.h"

struct battery_shader {
	shader shader;
	// List of RGB pointers
	RGB *high_colors;
	RGB *low_colors ;
};

// Make sure that this is actually defined somewhere
// Currently it works because main defines it. And
// that's the only place which includes this file.
extern batteryData batData;


static void init_shader(battery_shader*, unsigned int, unsigned int);
static void free_shader(battery_shader*);

// TODO should const also be noted here?
static void hsvGradient(battery_shader*, byte[4], uint, uint, ulong);
static void run_shader(battery_shader*, byte[4], uint, uint, ulong);

static int getHighPos(int, int, int, unsigned int);

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
static int getHighPos(int loop, int speed, int glider_width, unsigned int screen_width) {
	int sideWidth = glider_width / 2;

	int highPos;
	if (batData.status == CHARGING) {
		highPos = ((loop * speed) % (screen_width + glider_width)) - sideWidth;
	} else { // if (batData.status == DISCHARGING) {
		highPos = screen_width - ((loop * speed) % (screen_width + glider_width)) - sideWidth;
	}
	return highPos;
}

/*
 * TODO clean up obejects
 */
static void init_shader(battery_shader *shader,
		unsigned int width, unsigned int height) {

	shader->shader.width = width;

	// List of RGB pointers
	shader->high_colors = malloc(sizeof(RGB) * width);
	shader->low_colors  = malloc(sizeof(RGB) * width);

	HSV hsv;

	for (int x = 0; x < width; x++) {
		hsv.h = (1.0/3) * ((double) x/width);
		hsv.s = 1;
		hsv.v = 0.6;

		RGB rgb1;
		hsv_to_rgb (&hsv, &rgb1);
		shader->low_colors [x] = rgb1;

		RGB rgb2;
		hsv.v = 1;
		hsv_to_rgb (&hsv, &rgb2);
		shader->high_colors [x] = rgb2;
	}
}

static void free_shader(battery_shader *shader) {
	free (shader->high_colors);
	free (shader->low_colors);
	free (shader);
}

static void hsvGradient(
		battery_shader *shader,
		byte pixel[4],
		const uint x,
		const uint y,
		const ulong loop)
{
	// H :: 0 -> 1/3
	// Goes from red to green via yellow

	int highPos = getHighPos (loop, 10, 70, shader->shader.width);

	RGB rgb;

	double vval;
	if (batData.status == FULL) {
		rgb = shader->high_colors [x];
	} else if ( (vval = gliderValueHelper(loop, x, y, 10, 70, 0.7, highPos)) != -1 ) {
		HSV hsv;
		hsv.h = (1.0/3) * ((double) x/shader->shader.width);
		hsv.s = 1;
		hsv.v = gliderValueHelper(loop, x, y, 10, 70, 0.6, highPos);
		hsv_to_rgb(&hsv, &rgb);
	} else {
		rgb = shader->low_colors [x];
	}

	pixel [R] = rgb.r;
	pixel [G] = rgb.g;
	pixel [B] = rgb.b;
}

/*
 * Calls hsvGradient, but replaces with black if "outside"
 * battery range.
 */
static void run_shader(
		battery_shader *shader,
		byte pixel[4],
		const uint x,
		const uint y,
		const ulong loop)
{
	hsvGradient(shader, pixel, x, y, loop);
	if (x > batData.rate * shader->shader.width) {
		pixel [R] = 0;
		pixel [G] = 0;
		pixel [B] = 0;
	}
}

shader *create_battery_shader () {
	battery_shader *sh = malloc(sizeof(*sh));
	sh->shader.init = (init_t) init_shader;
	sh->shader.cleanup = (cleanup_t) free_shader;
	sh->shader.shader = (run_t) run_shader;
	return (shader*) sh;
}

#include "bad_shaders.h"

void repeatingGradient(
		byte* pixel,
		const uint x,
		const uint y,
		const ulong loop)
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
		const uint x,
		const uint y,
		const ulong loop)
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

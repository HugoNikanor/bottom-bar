#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "hsvrgb.h"
/*
 * Might round the wrong way
 *
 * taken from
 * http://www.easyrgb.com/en/math.php
 */
void hsv_to_rgb(HSV* hsv, RGB* rgb) {
	double r, g, b;

	//RGB* rgb = malloc(sizeof(RGB));
	if (hsv->s == 0) {
		r = hsv->v;
		g = hsv->v;
		b = hsv->v;
	} else {
		double h = hsv->h * 6;
		if (h == 6) h = 0; // H < 1 (?)
		int i = h;
		double v1 = hsv->v * (1.0 - hsv->s);
		double v2 = hsv->v * (1.0 - hsv->s * (h - i));
		double v3 = hsv->v * (1.0 - hsv->s * (1 - (h - i)));

		switch (i) {
			case 0:
				r = hsv->v;
				g = v3;
				b = v1;
				break;
			case 1:
				r = v2;
				g = hsv->v;
				b = v1;
				break;
			case 2:
				r = v1;
				g = hsv->v;
				b = v3;
				break;
			case 3:
				r = v1;
				g = v2;
				b = hsv->v;
				break;
			case 4:
				r = v3;
				g = v1;
				b = hsv->v;
				break;
			default:
				r = hsv->v;
				g = v1;
				b = v2;

		}
	}
	rgb->r = r * 0xFF;
	rgb->g = g * 0xFF;
	rgb->b = b * 0xFF;

	//return rgb;
}

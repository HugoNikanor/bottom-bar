#include <stdio.h>

#include "hsvrgb.h"

void test_single(double h, double s, double v) {
	struct HSV hsv;
	hsv.h = h;
	hsv.s = s;
	hsv.v = v;

	struct RGB rgb = *hsv_to_rgb(&hsv);

	//printf("r: %d, g: %d, b: %d\n", rgb.r, rgb.g, rgb.b);
	printf("r: %i, g: %i, b: %i\n", rgb.r, rgb.g, rgb.b);
}

void test_single_all() {

	test_single(0, 0, 0);
	test_single(0, 0, 1);
	test_single(0, 1, 1);
	test_single(1.0/3, 1, 1);
	test_single(2.0/3, 1, 1);
	test_single(1.0/6, 1, 1);
	test_single(0.5, 1, 1);
	test_single(5.0/6, 1, 1);

	//printf("%d\n", -DBL_MAX);
	printf("--------------------------------------------------\n");

	test_single(0, 0, 0.75);
	test_single(0, 0, 0.5);
	test_single(0, 1, 0.5);
	test_single(1.0/6,1, 0.5);
	test_single(1.0/3,1,0.5);
	test_single(5.0/6, 1, 0.5);
	test_single(0.5,1,0.5);
	test_single(2.0/3, 1, 0.5);
}

int main() {
	test_single(0.321615, 1, 1);
}

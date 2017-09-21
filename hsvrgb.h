#ifndef HSVRGB_HEADER
#define HSVRGB_HEADER

struct HSV {
	double h;
	double s;
	double v;
};

struct RGB {
	int r;
	int g;
	int b;
};

struct RGB* hsv_to_rgb(struct HSV* hsv);

#endif // HSVRGB_HEADER

#ifndef HSVRGB_HEADER
#define HSVRGB_HEADER

typedef struct {
	double h;
	double s;
	double v;
} HSV;

typedef struct {
	int r;
	int g;
	int b;
} RGB;

void hsv_to_rgb(HSV*, RGB*);

#endif // HSVRGB_HEADER

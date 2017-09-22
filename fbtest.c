#include <stdio.h>
#include <stdlib.h>

// open
#include <fcntl.h>
// ioctl
#include <stropts.h>

#include <linux/fb.h>
#include <linux/kd.h> 

int main() {
	struct fb_fix_screeninfo finfo;
	struct fb_var_screeninfo vinfo;

	int fb_fd = open("/dev/fb0", O_RDWR);

	ioctl(fb_fd, FBIOGET_VSCREENINFO, &vinfo);

	printf("linelength: %li\n", finfo.line_length);
	printf("mem-length: %li\n", finfo.smem_len);

	printf("xres: %i, yres: %i\n", vinfo.xres, vinfo.yres);
	printf("bps: %i\n", vinfo.bits_per_pixel);
	printf("width: %i, height: %i\n", vinfo.width, vinfo.height);


	/*
	struct consolefentdesc fontdesc;
	ioctl(fd_fb, FBIOG
	*/
}

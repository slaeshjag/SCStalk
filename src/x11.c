#include "xreenstr.h"

#include <stdio.h>
#include <errno.h>
#include <X11/Xos.h>
#include <stdlib.h>

#include <X11/Xlib.h>
#include <X11/Xutil.h>

#define	SCREEN_NUMBER		0


unsigned int x11ShiftOffset(unsigned int mask) {
	int i;

	if (mask == 0)
		return 0;
	for (i = 0; !(mask & 0x1); i++, mask >>= 1);
	return i;
}


void x11ConvertToRGBA32(XImage *image) {
	int i, rs, gs, bs;
	unsigned int *big, bt;
	unsigned short *small, st;

	big = (unsigned int *) image->data;
	small = (unsigned short *) image->data;
	rs = shiftOffset(image->red_mask);
	gs = shiftOffset(image->green_mask);
	bs = shiftOffset(image->blue_mask);
	
	for (i = 0; i < image->width * image->height; i++) {
		if (image->bits_per_pixel == 32) {
			bt = big[i];
			big[i] = (bt & image->red_mask) >> shiftOffset(image->red_mask);
			big[i] |= ((bt & image->green_mask) >> shiftOffset(image->green_mask) << 8);
			big[i] |= ((bt & image->blue_mask) >> shiftOffset(image->blue_mask) << 16);
			big[i] |= 0xFF000000;
		} else if (image->bits_per_pixel == 16) {
			/* TODO */
		}
	}

	return;
}





int screenGrab(const char *display, int window_number, XS_IMAGE *image) {
	int i;
	XImage *ximage;
	Display *dpy;
	Drawable d;
	int w, h;

	dpy = XOpenDisplay(":0");
	d = RootWindow(dpy, SCREEN_NUMBER);
	ximage = XGetImage(dpy, d, 0, 0, 640, 480, AllPlanes, ZPixmap);

	x11ConvertToRGBA32(ximage);
	x11CopyToImage(image, ximage);
	
	(ximage->f.destroy_image)();
	
	fwrite(image->data, 640*480, image->bits_per_pixel, stdout);
	fprintf(stderr, "Depth: %i, %i\n", image->depth, image->bits_per_pixel);
	
	return 0;
}

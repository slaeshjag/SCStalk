#include "SCStalk.h"

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


void x11ConvertToRGBA32(XImage *ximage, XS_IMAGE *image) {
	int i, rs, gs, bs;
	unsigned int *big, bt;

	big = (unsigned int *) ximage->data;

	rs = x11ShiftOffset(ximage->red_mask);
	gs = x11ShiftOffset(ximage->green_mask);
	bs = x11ShiftOffset(ximage->blue_mask);
	
	for (i = 0; i < ximage->width * ximage->height; i++) {
		if (ximage->bits_per_pixel == 32) {
			bt = big[i];
			image->data[i] = (bt & ximage->red_mask) >> rs;
			image->data[i] |= ((bt & ximage->green_mask) >> gs) << 8;
			image->data[i] |= ((bt & ximage->blue_mask) >> bs) << 16;
			image->data[i] |= 0xFF000000;
		} else {
			fprintf(stderr, "Only 24/32 bpp is currently supported\n");
			exit(-1);
		}
	}

	return;
}


int screenValidate(const char *display, int screen_number, int x, int y, int w, int h) {
	Display *dpy;
	if (display == NULL)
		display = ":0";
	
	if ((dpy = XOpenDisplay(display)) == NULL) {
		fprintf(stderr, "X11: Unable to open display %s\n", display);
		return -1;
	}

	if (x + w > DisplayWidth(dpy, screen_number))
		return -1;
	if (y + h > DisplayHeight(dpy, screen_number))
		return -1;
	
	return 0;
}


int screenGrab(const char *display, int screen_number, XS_IMAGE *image) {
	XImage *ximage;
	Display *dpy;
	Drawable d;

	if (display == NULL)
		display = ":0";

	dpy = XOpenDisplay(display);
	d = RootWindow(dpy, screen_number);
	ximage = XGetImage(dpy, d, image->x, image->y, image->w, image->h, AllPlanes, ZPixmap);

	x11ConvertToRGBA32(ximage, image);
	(ximage->f.destroy_image)(ximage);
	
	return 0;
}

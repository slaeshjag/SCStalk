#include "SCStalk.h"

XS_IMAGE *xreenstrImageNew(unsigned int x, unsigned int y, unsigned int w, unsigned int h) {
	XS_IMAGE *image;

	if ((image = malloc(sizeof(XS_IMAGE))) == NULL)
		return NULL;
	
	if ((image->data = malloc(sizeof(unsigned int) * w * h)) == NULL) {
		free(image);
		return NULL;
	}

	image->x = x;
	image->y = y;
	image->w = w;
	image->h = h;

	return image;
}


int main(int argc, char **argv) {
	XS_IMAGE *image;

	image = xreenstrImageNew(400, 400, 800, 600);
	screenGrab(NULL, 0, image);

	fwrite(image->data, image->w * image->h, 4, stdout);

	return 0;
}


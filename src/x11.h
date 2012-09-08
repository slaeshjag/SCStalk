#ifndef __X11_XREENSTR_H__
#define	__X11_XREENSTR_H__

int screenValidate(const char *display, int screen_number, int x, int y, int w, int h);
int screenGrab(const char *display, int screen_number, XS_IMAGE *image);

#endif

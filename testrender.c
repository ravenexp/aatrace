#include <stdio.h>
#include <stdlib.h>

#include "pnm.h"
#include "aatrace.h"
#include "util.h"

int main(int argc, char* argv[])
{
	char* txtbuf;
	int twidth, theight;

	pix_t* fbuf;
	int fwidth, fheight;

	pix_t* obuf;
	int width, height;

	if (argc < 4)
		return 1;

	txtbuf = util_load_text(argv[1], &twidth, &theight);
	if (!txtbuf)
		return 2;

	fbuf = util_load_pic(argv[2], &fwidth, &fheight);
	if (!fbuf || fwidth != AATRACE_FONT_WIDTH)
		return 3;

	width = twidth*AATRACE_FONT_WIDTH;
	height = theight*AATRACE_FONT_HEIGHT;

	obuf = (pix_t*)malloc(width*height);
	aatrace_render_pic(obuf, txtbuf, fbuf,
			   width,
			   twidth, theight,
			   fwidth, AATRACE_FONT_HEIGHT);

	if (util_store_pic(argv[3], obuf, width, height) < 0)
		return 4;

	return 0;
}

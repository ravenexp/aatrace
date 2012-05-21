#include <stdio.h>
#include <stdlib.h>

#include "pnm.h"
#include "aatrace.h"
#include "util.h"

int main(int argc, char* argv[])
{
	pix_t* fbuf;
	pix_t* sbuf;
	pix_t* dbuf;
	pix_t* obuf;
	int width, height;
	int fwidth, fheight;

	char* txtbuf;
	int twidth, theight;

	if (argc < 3)
		return 1;

	sbuf = util_load_pic(argv[1], &width, &height);
	if (!sbuf)
		return 2;

	dbuf = (pix_t*)malloc(width*height);
	aatrace_diff(dbuf, sbuf, width, height);
	util_store_pic(argv[2], dbuf, width, height);

	if (argc < 5)
		return 3;

	fbuf = util_load_pic(argv[3], &fwidth, &fheight);
	if (!fbuf || fwidth != AATRACE_FONT_WIDTH)
		return 4;

	twidth = width/AATRACE_FONT_WIDTH;
	theight = height/AATRACE_FONT_HEIGHT;

	txtbuf = (char*)malloc(twidth*theight);
	aatrace_match_pic(txtbuf, dbuf, fbuf,
			  width, height,
			  fwidth, fheight, AATRACE_FONT_HEIGHT);

	util_store_text(argv[4], txtbuf, twidth, theight);

	obuf = (pix_t*)malloc(width*height);
	aatrace_render_pic(obuf, txtbuf, fbuf,
			   width,
			   twidth, theight,
			   fwidth, AATRACE_FONT_HEIGHT);

	if (util_store_pic(argv[5], obuf, width, height) < 0)
		return 5;

	return 0;
}

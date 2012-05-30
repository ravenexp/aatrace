#include <stdio.h>
#include <stdlib.h>

#include "pnm.h"
#include "aatrace.h"
#include "util.h"

int main(int argc, char* argv[])
{
	pix_t* fbuf;
	pix_t* sbuf;
	int width, height;
	int fwidth, fheight;

	char* txtbuf;
	int twidth, theight;

	if (argc < 4)
		return 1;

	sbuf = util_load_pic(argv[1], &width, &height);
	if (!sbuf)
		return 2;

	fbuf = util_load_pic(argv[2], &fwidth, &fheight);
	if (!fbuf || fwidth != AATRACE_FONT_WIDTH)
		return 3;

	twidth = width/AATRACE_FONT_WIDTH;
	theight = height/AATRACE_FONT_HEIGHT;

	txtbuf = (char*)malloc(twidth*theight);
	aatrace_match_pic(txtbuf, sbuf, fbuf,
			  width, height,
			  fwidth, fheight, AATRACE_FONT_HEIGHT);

	if (util_store_text(argv[3], txtbuf, twidth, theight) < 0)
		return 4;

	return 0;
}

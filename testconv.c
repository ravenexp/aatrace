#include <stdio.h>
#include <stdlib.h>

#include "pnm.h"
#include "aatrace.h"
#include "util.h"

int main(int argc, char* argv[])
{
	struct aatrace_pic src, font;
	char* txtbuf;
	int twidth, theight;

	if (argc < 4)
		return 1;

	if (util_load_pic(&src, argv[1]) < 0)
		return 2;

	if (util_load_pic(&font, argv[2]) < 0)
		return 3;
	if (font.w != AATRACE_FONT_WIDTH)
		return 4;

	twidth = src.w/AATRACE_FONT_WIDTH;
	theight = src.h/AATRACE_FONT_HEIGHT;

	txtbuf = (char*)malloc(twidth*theight);
	aatrace_match_pic(txtbuf, &src, &font, AATRACE_FONT_HEIGHT);

	if (util_store_text(argv[3], txtbuf, twidth, theight) < 0)
		return 5;

	return 0;
}

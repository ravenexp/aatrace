#include <stdio.h>
#include <stdlib.h>

#include "pnm.h"
#include "aatrace.h"
#include "util.h"

int main(int argc, char* argv[])
{
	struct aatrace_pic out, font;

	char* txtbuf;
	int twidth, theight;

	if (argc < 4)
		return 1;

	txtbuf = util_load_text(argv[1], &twidth, &theight);
	if (!txtbuf)
		return 2;

	if (util_load_pic(&font, argv[2]) < 0)
		return 3;
	if (font.w != AATRACE_FONT_WIDTH)
		return 4;

	aatrace_render_pic(&out, txtbuf, &font,
			   twidth, theight,
			   AATRACE_FONT_HEIGHT);

	if (util_store_pic(argv[3], &out) < 0)
		return 5;

	return 0;
}

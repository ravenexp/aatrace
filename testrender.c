#include <stdio.h>
#include <stdlib.h>

#include "pnm.h"
#include "aatrace.h"
#include "util.h"

int main(int argc, char* argv[])
{
	struct aatrace_pic out;
	struct aatrace_font font;

	char* txtbuf;
	int twidth, theight;

	if (argc < 4)
		return 1;

	txtbuf = util_load_text(argv[1], &twidth, &theight);
	if (!txtbuf)
		return 2;

	if (util_load_font(&font, argv[2]) < 0)
		return 3;

	aatrace_render_pic(&out, txtbuf, &font,
			   twidth, theight);

	if (util_store_pic(argv[3], &out) < 0)
		return 4;

	return 0;
}

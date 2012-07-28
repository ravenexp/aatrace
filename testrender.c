#include <stdio.h>
#include <stdlib.h>

#include "pnm.h"
#include "aatrace.h"
#include "util.h"

int main(int argc, char* argv[])
{
	struct aatrace_text txt;
	struct aatrace_font font;
	struct aatrace_pic out;

	if (argc < 4)
		return 1;

	if (util_load_text(&txt, argv[1]) < 0)
		return 2;

	if (util_load_font(&font, argv[2]) < 0)
		return 3;

	aatrace_render_pic(&out, &txt, &font);

	if (util_store_pic(argv[3], &out) < 0)
		return 4;

	return 0;
}

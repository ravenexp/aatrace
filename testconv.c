#include <stdio.h>
#include <stdlib.h>

#include "pnm.h"
#include "aatrace.h"
#include "util.h"

int main(int argc, char* argv[])
{
	struct aatrace_pic src;
	struct aatrace_font font;
	struct aatrace_text txt;

	if (argc < 4)
		return 1;

	if (util_load_pic(&src, argv[1]) < 0)
		return 2;

	if (util_load_font(&font, argv[2]) < 0)
		return 3;

	aatrace_match_pic(&txt, &src, &font);

	if (util_store_text(argv[3], &txt) < 0)
		return 4;

	return 0;
}

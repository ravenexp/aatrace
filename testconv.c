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

	struct aatrace_match_ctx ctx;

	if (argc < 4)
		return 1;

	if (util_load_pic(&src, argv[1]) < 0)
		return 2;

	if (util_load_font(&font, argv[2]) < 0)
		return 3;

	ctx.method = AATRACE_MATCH_METHOD_DEFAULT;
	if (argc > 4)
		ctx.method = (enum aatrace_match_method)atoi(argv[4]);
	ctx.sad_weight = ctx.asd_weight = 0;

	aatrace_match_pic(&txt, &src, &font, ctx);

	if (util_store_text(argv[3], &txt) < 0)
		return 4;

	return 0;
}

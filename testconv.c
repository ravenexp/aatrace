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

	struct aatrace_convert_ctx ctx;

	if (argc < 4)
		return 1;

	if (util_load_pic(&src, argv[1]) < 0)
		return 2;

	if (util_load_font(&font, argv[2]) < 0)
		return 3;

	ctx.match.method = AATRACE_MATCH_METHOD_DEFAULT;
	if (argc > 4)
		ctx.match.method = (enum aatrace_match_method)atoi(argv[4]);

	ctx.match.sad_weight = ctx.match.asd_weight = 0;
	if (argc > 5)
		ctx.match.sad_weight = atoi(argv[5]);
	if (argc > 6)
		ctx.match.asd_weight = atoi(argv[6]);

	aatrace_convert_pic(&txt, &src, &font, ctx);

	if (util_store_text(argv[3], &txt) < 0)
		return 4;

	return 0;
}

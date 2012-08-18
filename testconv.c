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

	aatrace_convert_ctx_init(&ctx);

	if (argc < 4)
		return 1;

	if (util_load_pic(&src, argv[1]) < 0)
		return 2;

	if (util_load_font(&font, argv[2]) < 0)
		return 3;

	if (argc > 4)
		ctx.match.method = (enum aatrace_match_method)atoi(argv[4]);

	if (argc > 5)
		ctx.search.method = (enum aatrace_search_method)atoi(argv[5]);

	if (argc > 6)
		ctx.search.flags = atoi(argv[6]);

	if (argc > 7)
		ctx.match.sad_weight = atoi(argv[7]);
	if (argc > 8)
		ctx.match.asd_weight = atoi(argv[8]);

	aatrace_convert_pic(&txt, &src, &font, &ctx);

	if (util_store_text(argv[3], &txt) < 0)
		return 4;

	return 0;
}

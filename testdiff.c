#include <stdio.h>
#include <stdlib.h>

#include "pnm.h"
#include "aatrace.h"
#include "util.h"

int main(int argc, char* argv[])
{
	struct aatrace_pic src, dst;
	struct aatrace_diff_ctx ctx;

	aatrace_diff_ctx_init(&ctx);

	if (argc < 3)
		return 1;

	if (util_load_pic(&src, argv[1]) < 0)
		return 2;

	if (argc > 3)
		ctx.scale = atoi(argv[3]);

	if (argc > 4)
		ctx.kernel = (enum aatrace_diff_kernel)atoi(argv[4]);

	aatrace_diff(&dst, &src, &ctx);
	util_store_pic(argv[2], &dst);

	return 0;
}

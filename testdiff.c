#include <stdio.h>
#include <stdlib.h>

#include "pnm.h"
#include "aatrace.h"
#include "util.h"

int main(int argc, char* argv[])
{
	struct aatrace_pic src, dst;
	struct aatrace_diff_ctx ctx;

	if (argc < 3)
		return 1;

	if (util_load_pic(&src, argv[1]) < 0)
		return 2;

	ctx.scale = 0;
	if (argc > 3)
		ctx.scale = atoi(argv[3]);

	ctx.kernel = AATRACE_DIFF_KERNEL_DEFAULT;
	if (argc > 4)
		ctx.kernel = (enum aatrace_diff_kernel)atoi(argv[4]);

	aatrace_diff(&dst, &src, ctx);
	util_store_pic(argv[2], &dst);

	return 0;
}

#include <stdio.h>
#include <stdlib.h>

#include <getopt.h>

#include "pnm.h"
#include "aatrace.h"
#include "util.h"

static const char usage[] =
"Usage:\n"
"    testdiff [-l <scale>] [-d <kernel>] [-t <thresh>] input.pgm output.pgm";

int main(int argc, char* argv[])
{
	struct aatrace_pic src, dst;
	struct aatrace_diff_ctx ctx;
	int c;

	aatrace_diff_ctx_init(&ctx);

	while ((c = getopt(argc, argv, ":l:d:t:")) != -1) {
		switch(c) {
		case 'l':
			ctx.scale = atoi(optarg);
			break;
		case 'd':
			ctx.kernel = (enum aatrace_diff_kernel)atoi(optarg);
			break;
		case 't':
			ctx.threshold = (unsigned)atoi(optarg);
			break;
		case ':':
			fprintf(stderr, "Option -%c requires an operand\n", optopt);
			return 2;
		default:
		case '?':
			fprintf(stderr, "Unrecognized option: -%c\n", optopt);
			return 1;
		}
	}

	if (optind != argc - 2) {
		puts(usage);
		return 3;
	}

	if (util_load_pic(&src, argv[optind]) < 0)
		return 4;

	aatrace_diff(&dst, &src, &ctx);

	if (util_store_pic(argv[optind + 1], &dst) < 0)
		return 5;

	return 0;
}

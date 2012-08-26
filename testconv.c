#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>

#include "pnm.h"
#include "aatrace.h"
#include "util.h"

static const char usage[] =
"Usage:\n"
"    testconv OPTIONS input.pgm font.pgm output.pgm\n"
"OPTIONS:\n"
"    [-m <id>] [-W <sad>,<asd>] [-s <id>] [-r (<px>|<hpx>,<vpx>)] [-c <1|0>]";

int main(int argc, char* argv[])
{
	struct aatrace_pic src;
	struct aatrace_font font;
	struct aatrace_text txt;

	struct aatrace_convert_ctx ctx;
	int c;

	aatrace_convert_ctx_init(&ctx);

	while ((c = getopt(argc, argv, ":m:W:s:r:c:")) != -1) {
		const char* p;

		switch(c) {
		case 'm':
			ctx.match.method = (enum aatrace_match_method)atoi(optarg);
			break;
		case 'W':
			ctx.match.sad_weight = atoi(optarg);
			p = strchr(optarg, ',');
			if (!p) {
				fputs("-W requires two operands: <sad>,<asd>", stderr);
				return 2;
			}
			ctx.match.asd_weight = atoi(p + 1);
			break;
		case 's':
			ctx.search.method = (enum aatrace_search_method)atoi(optarg);
			break;
		case 'r':
			ctx.search.hrange = (unsigned)atoi(optarg);
			p = strchr(optarg, ',');
			ctx.search.vrange = p ? (unsigned)atoi(p + 1) : ctx.search.hrange;
			break;
		case 'c':
			if (atoi(optarg)) {
				ctx.search.flags |= AATRACE_SEARCH_FLAG_COVERAGE;
			} else {
				ctx.search.flags &= ~AATRACE_SEARCH_FLAG_COVERAGE;
			}
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

	if (optind != argc - 3) {
		puts(usage);
		return 3;
	}

	if (util_load_pic(&src, argv[optind]) < 0)
		return 4;

	if (util_load_font(&font, argv[optind + 1]) < 0)
		return 5;

	aatrace_convert_pic(&txt, &src, &font, &ctx);

	if (util_store_text(argv[optind + 2], &txt) < 0)
		return 6;

	return 0;
}

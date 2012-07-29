#include <stdlib.h>
#include "aatrace.h"
#include "match.h"

static
struct match_result
aatrace_match_tile_sad(const unsigned char* tilebuf,
		       const unsigned char* fontbuf,
		       int nchars,
		       struct aatrace_match_ctx ctx)
{
	struct match_result r;
	ctx.sad_weight = ctx.asd_weight = 0;

	unsigned int minsad = 255*FW*FH;
	int best_ch = 0;

	int ch;

	for (ch = 0; ch < nchars; ch++) {
		int l, i;
		unsigned int sad = 0;

		for (l = 0; l < FH; l++)
			for (i = 0; i < FW; i++)
				sad += abs(  tilebuf[l*FW + i]
					   - fontbuf[(ch*FH + l)*FW + i]);

		if (sad < minsad) {
			minsad = sad;
			best_ch = ch;
		}
	}

	r.ch = best_ch;
	r.score = minsad;
	return r;
}

static
struct match_result
aatrace_match_tile_sadasd(const unsigned char* tilebuf,
			  const unsigned char* fontbuf,
			  int nchars,
			  struct aatrace_match_ctx ctx)
{
	struct match_result r;

	unsigned int minsadasd = (ctx.sad_weight + ctx.asd_weight)*255*FW*FH;
	int best_ch = 0;

	int ch;

	for (ch = 0; ch < nchars; ch++) {
		unsigned int sad = 0;
		int sd = 0;
		unsigned int asd;
		unsigned int sadasd;

		int l, i;

		for (l = 0; l < FH; l++) {
			for (i = 0; i < FW; i++) {
				int d = tilebuf[l*FW + i]
				      - fontbuf[(ch*FH + l)*FW + i];
				sd += d;
				sad += abs(d);
			}
		}

		asd = abs(sd);
		sadasd = sad*ctx.sad_weight + asd*ctx.asd_weight;
		if (sadasd < minsadasd) {
			minsadasd = sadasd;
			best_ch = ch;
		}
	}

	r.ch = best_ch;
	r.score = minsadasd;
	return r;
}

struct match_result
aatrace_match_tile(const unsigned char* tilebuf,
		   const unsigned char* fontbuf,
		   int nchars,
		   struct aatrace_match_ctx ctx)
{
	if (!ctx.sad_weight)
		ctx.sad_weight = AATRACE_DEFAULT_SAD_WEIGHT;
	if (!ctx.asd_weight)
		ctx.asd_weight = AATRACE_DEFAULT_ASD_WEIGHT;

	if (ctx.method == AATRACE_MATCH_METHOD_DEFAULT)
		ctx.method = AATRACE_DEFAULT_MATCH_METHOD;

	switch (ctx.method) {
	default:
	case AATRACE_MATCH_METHOD_MINSAD:
		return aatrace_match_tile_sad(tilebuf, fontbuf, nchars, ctx);
	case AATRACE_MATCH_METHOD_MINSADASD:
		return aatrace_match_tile_sadasd(tilebuf, fontbuf, nchars, ctx);
	}
}

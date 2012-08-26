#include <stdlib.h>
#include "aatrace.h"
#include "match.h"

static
struct match_result
aatrace_match_tile_sad(const unsigned char* tilebuf,
		       const unsigned char* fontbuf,
		       int nchars)
{
	struct match_result r;

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

		if (minsad == 0)
			break;
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
			  unsigned int sad_weight,
			  unsigned int asd_weight)
{
	struct match_result r;

	unsigned int minsadasd = (sad_weight + asd_weight)*255*FW*FH;
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
		sadasd = sad*sad_weight + asd*asd_weight;
		if (sadasd < minsadasd) {
			minsadasd = sadasd;
			best_ch = ch;
		}

		if (minsadasd == 0)
			break;
	}

	r.ch = best_ch;
	r.score = minsadasd;
	return r;
}

void aatrace_match_ctx_init(struct aatrace_match_ctx* ctx)
{
	ctx->sad_weight = AATRACE_DEFAULT_SAD_WEIGHT;
	ctx->asd_weight = AATRACE_DEFAULT_ASD_WEIGHT;
	ctx->method = AATRACE_DEFAULT_MATCH_METHOD;
}

struct match_result
aatrace_match_tile(const unsigned char* tilebuf,
		   const unsigned char* fontbuf,
		   int nchars,
		   const struct aatrace_match_ctx* ctx)
{
	switch (ctx->method) {
	default:
	case AATRACE_MATCH_METHOD_MINSAD:
		return aatrace_match_tile_sad(tilebuf, fontbuf, nchars);
	case AATRACE_MATCH_METHOD_MINSADASD:
		return aatrace_match_tile_sadasd(tilebuf, fontbuf, nchars,
						 ctx->sad_weight, ctx->asd_weight);
	}
}

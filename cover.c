#include <stdlib.h>
#include "aatrace.h"
#include "match.h"

static
unsigned int edge_score(const unsigned char* p, int l,
			unsigned int stride,
			struct aatrace_match_ctx ctx)
{
	unsigned int score = 0;
	int i;

	for (i = 0; i < l; i++) {
		score += p[i*stride];
	}

	if (ctx.method == AATRACE_MATCH_METHOD_MINSADASD)
		score *= (ctx.sad_weight + ctx.asd_weight);

	return score;
}

/* Assumes FW == font->w, FH == font->h */
unsigned int
aatrace_cover_tile_score(const struct aatrace_pic* pic,
			 const struct aatrace_font* font,
			 int tl, int tc, int dx, int dy,
			 struct aatrace_match_ctx ctx)
{
	const unsigned char* p =
		pic->buf + font->h*pic->ll*tl + font->w*tc;

	unsigned int score = 0;

	if (dy == 1)
		score += edge_score(p, FW, 1, ctx);
	if (dy == -1)
		score += edge_score(p + (font->h - 1)*pic->ll, FW, 1, ctx);

	if (dx == 1)
		score += edge_score(p, FH, pic->ll, ctx);
	if (dx == -1)
		score += edge_score(p + font->w - 1, FH, pic->ll, ctx);

	return score;
}

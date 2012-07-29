#include <stdlib.h>
#include "aatrace.h"
#include "match.h"
#include "bitblt.h"

/* Assumes FW == font->w, FH == font->h */
static
int aatrace_search_tile_none(const struct aatrace_pic* src,
			     const struct aatrace_font* font,
			     int tl, int tc,
			     struct aatrace_convert_ctx ctx)
{
	unsigned char tilebuf[FH][FW];
	struct match_result r;

	aatrace_bitblt(&tilebuf[0][0], src->buf,
		       FW, src->ll,
		       0, 0, tc*FW, tl*FH,
		       FW, FH);

	r = aatrace_match_tile(&tilebuf[0][0], font->pic.buf,
			       font->nchars, ctx.match);

	return r.ch;
}

static
int aatrace_search_tile_diamondbox(const struct aatrace_pic* src,
				   const struct aatrace_font* font,
				   int tl, int tc,
				   struct aatrace_convert_ctx ctx)
{
	unsigned char tilebuf[FH][FW];
	struct match_result r, best_r = {0, -1};
	int dx, dy;

	for (dy = -1; dy <= 1; dy++)
		for (dx = -1; dx <= 1; dx++) {
			if (tc*FW + dx < 0 || (tc + 1)*FW + dx > src->w)
				continue;
			if (tl*FH + dy < 0 || (tl + 1)*FH + dy > src->h)
				continue;
			if (ctx.search.method == AATRACE_SEARCH_METHOD_DIAMOND &&
			    dx*dy != 0)
				continue;

			aatrace_bitblt(&tilebuf[0][0], src->buf,
				       FW, src->ll,
				       0, 0, tc*FW + dx, tl*FH + dy,
				       FW, FH);

			r = aatrace_match_tile(&tilebuf[0][0], font->pic.buf,
					       font->nchars, ctx.match);

			if (r.score < best_r.score)
				best_r = r;
		}

	return best_r.ch;
}

int aatrace_search_tile(const struct aatrace_pic* src,
			const struct aatrace_font* font,
			int tl, int tc,
			struct aatrace_convert_ctx ctx)
{
	if (ctx.search.method == AATRACE_SEARCH_METHOD_DEFAULT)
		ctx.search.method = AATRACE_DEFAULT_SEARCH_METHOD;

	switch (ctx.search.method) {
	default:
	case AATRACE_SEARCH_METHOD_NONE:
		return aatrace_search_tile_none(src, font, tl, tc, ctx);
	case AATRACE_SEARCH_METHOD_DIAMOND:
	case AATRACE_SEARCH_METHOD_BOX:
		return aatrace_search_tile_diamondbox(src, font, tl, tc, ctx);
	}
}

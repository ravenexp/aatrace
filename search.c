#include <stdlib.h>
#include "aatrace.h"
#include "match.h"
#include "search.h"
#include "bitblt.h"

void aatrace_search_ctx_init(struct aatrace_search_ctx* ctx)
{
	ctx->method = AATRACE_DEFAULT_SEARCH_METHOD;
	ctx->flags =  AATRACE_DEFAULT_SEARCH_FLAGS;
	ctx->hrange = AATRACE_DEFAULT_SEARCH_HRANGE;
	ctx->vrange = AATRACE_DEFAULT_SEARCH_VRANGE;
}

/* Assumes FW == font->w, FH == font->h */
int aatrace_search_tile(const struct aatrace_pic* src,
			const struct aatrace_font* font,
			int tl, int tc,
			const struct aatrace_match_ctx* m_ctx,
			const struct aatrace_search_ctx* ctx)
{
	unsigned char tilebuf[FH][FW];
	struct match_result r, best_r = {0, -1};
	int dx, dy;
	int xr, yr;

	switch (ctx->method) {
	case AATRACE_SEARCH_METHOD_DIAMOND:
	case AATRACE_SEARCH_METHOD_BOX:
		xr = yr = 1;
		break;

	case AATRACE_SEARCH_METHOD_AREA:
		xr = ctx->hrange;
		yr = ctx->vrange;
		break;

	case AATRACE_SEARCH_METHOD_NONE:
	default:
		xr = yr = 0;
	}

	for (dy = -yr; dy <= yr; dy++)
		for (dx = -xr; dx <= xr; dx++) {
			if (tc*FW + dx < 0 || (tc + 1)*FW + dx > src->w)
				continue;
			if (tl*FH + dy < 0 || (tl + 1)*FH + dy > src->h)
				continue;

			if (ctx->method == AATRACE_SEARCH_METHOD_DIAMOND &&
			    dx*dy != 0)
				continue;

			aatrace_bitblt(&tilebuf[0][0], src->buf,
				       FW, src->ll,
				       0, 0, tc*FW + dx, tl*FH + dy,
				       FW, FH);

			r = aatrace_match_tile(&tilebuf[0][0], font->pic.buf,
					       font->nchars, m_ctx);

			if (ctx->flags & AATRACE_SEARCH_FLAG_COVERAGE) {
				r.score += aatrace_cover_tile_score(src, font, tl, tc, dx, dy, m_ctx);
			}

			if (r.score < best_r.score)
				best_r = r;

			if (best_r.score == 0)
				break;
		}

	return best_r.ch;
}

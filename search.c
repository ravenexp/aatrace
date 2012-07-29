#include <stdlib.h>
#include "aatrace.h"
#include "match.h"
#include "bitblt.h"

/* Assumes FW == font->w, FH == font->h */

/* SEARCH_METHOD_NONE =) */
int aatrace_search_tile(const struct aatrace_pic* src,
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

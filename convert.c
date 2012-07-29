#include <stdlib.h>
#include "aatrace.h"
#include "search.h"

void aatrace_convert_pic(struct aatrace_text* txt,
			 const struct aatrace_pic* src,
			 const struct aatrace_font* font,
			 struct aatrace_convert_ctx ctx)
{
	int tl, tc;

	txt->ll = txt->w = src->w/AATRACE_FONT_WIDTH;
	txt->h = src->h/AATRACE_FONT_HEIGHT;
	txt->buf = (char*)malloc(txt->ll*txt->h);

	for (tl = 0; (tl + 1)*font->h <= src->h; tl++) {
		for (tc = 0; (tc + 1)*font->w <= src->w; tc++) {
			txt->buf[tl*txt->ll + tc] =
				aatrace_search_tile(src, font, tl, tc, ctx)
				+ font->ascii_offset;
		}
	}
}

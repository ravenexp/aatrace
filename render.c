#include <stdlib.h>
#include "aatrace.h"
#include "bitblt.h"

void aatrace_render_pic(struct aatrace_pic* out,
			const struct aatrace_text* txt,
			const struct aatrace_font* font)
{
	int tl, tc;

	out->ll = out->w = txt->w*font->w;
	out->h = txt->h*font->h;
	out->buf = (unsigned char*)malloc(out->ll*out->h);

	for (tl = 0; tl < txt->h; tl++)
		for (tc = 0; tc < txt->w; tc++) {
			int tile = txt->buf[tl*txt->ll + tc] - font->ascii_offset;

			aatrace_bitblt(out->buf, font->pic.buf,
				       out->ll, font->pic.ll,
				       tc*font->w, tl*font->h,
				       0, tile*font->h,
				       font->w, font->h);
		}
}


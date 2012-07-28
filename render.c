#include <stdlib.h>
#include "aatrace.h"

static
void aatrace_render_tile(unsigned char* tile, const unsigned char* font,
			 int ft, int w, int fw, int ch)
{
	int cl, cc;

	for (cl = 0; cl < ch; cl++)
		for (cc = 0; cc < fw; cc++)
			tile[cl*w + cc] = font[(ft*ch + cl)*fw + cc];
}


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
			int ft = txt->buf[tl*txt->ll + tc] - font->ascii_offset;

			aatrace_render_tile(&out->buf[tl*font->h*out->ll + tc*font->w],
					    font->pic.buf, ft, out->ll, font->w, font->h);
		}
}


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
			const char* txt,
			const struct aatrace_pic* font,
			int tw, int th, int char_h)
{
	int tl, tc;

	out->ll = out->w = tw*font->w;
	out->h = th*char_h;
	out->buf = (unsigned char*)malloc(out->ll*out->h);

	for (tl = 0; tl < th; tl++)
		for (tc = 0; tc < tw; tc++) {
			int ft = txt[tl*tw + tc] - AATRACE_FONT_ASCII_OFFSET;

			aatrace_render_tile(&out->buf[tl*char_h*out->ll + tc*font->w],
					    font->buf, ft, out->ll, font->w, char_h);
		}
}


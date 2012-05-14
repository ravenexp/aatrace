#include "aatrace.h"

void aatrace_render_tile(pix_t* t, const pix_t* f, int ft,
			 int w, int fw, int ch)
{
	int cl, cc;

	for (cl = 0; cl < ch; cl++)
		for (cc = 0; cc < fw; cc++)
			t[cl*w + cc] = f[(ft*ch + cl)*fw + cc];
}

void aatrace_render_pic(pix_t* p, const char* txt, const pix_t* f,
			int w, int tw, int th, int fw, int ch)
{
	int tl, tc;

	for (tl = 0; tl < th; tl++)
		for (tc = 0; tc < tw; tc++) {
			int ft = txt[tl*tw + tc] - AATRACE_FONT_ASCII_OFFSET;

			aatrace_render_tile(&p[tl*ch*w + tc*fw],
					    f, ft, w, fw, ch);
		}
}


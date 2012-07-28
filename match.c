#include <stdlib.h>
#include "aatrace.h"
#include "bitblt.h"

int aatrace_match_tile_sad(const unsigned char* tile, const unsigned char* font,
			   int fw, int fh, int ch)
{
	int ft, cl, cc;
	int minsad = 255*fw*ch;
	int best_ft = 0;

	for (ft = 0; ft*ch < fh - ch + 1; ft++) {
		int sad = 0;

		for (cl = 0; cl < ch; cl++)
			for (cc = 0; cc < fw; cc++)
				sad += abs(tile[cl*fw + cc] - font[(ft*ch + cl)*fw + cc]);

		if (sad < minsad) {
			minsad = sad;
			best_ft = ft;
		}
	}

	return best_ft;
}

int aatrace_match_tile_sadasd(const unsigned char* tile,
			      const unsigned char* font,
			      int fw, int fh, int ch)
{
	int ft, cl, cc;
	int minsadasd = 2*255*fw*ch;
	int best_ft = 0;

	for (ft = 0; ft*ch < fh - ch + 1; ft++) {
		int sad = 0;
		int sd = 0;
		int asd;

		for (cl = 0; cl < ch; cl++)
			for (cc = 0; cc < fw; cc++) {
				int d;

				d = tile[cl*fw + cc] - font[(ft*ch + cl)*fw + cc];
				sd += d;
				sad += abs(d);
			}

		asd = abs(sd);

		if (sad + asd < minsadasd) {
			minsadasd = sad + asd;
			best_ft = ft;
		}
	}

	return best_ft;
}

void aatrace_match_pic(struct aatrace_text* txt,
		       const struct aatrace_pic* src,
		       const struct aatrace_font* font)
{
	unsigned char tilebuf[AATRACE_FONT_HEIGHT][AATRACE_FONT_WIDTH];

	int tl, tc;

	txt->ll = txt->w = src->w/AATRACE_FONT_WIDTH;
	txt->h = src->h/AATRACE_FONT_HEIGHT;
	txt->buf = (char*)malloc(txt->ll*txt->h);

	for (tl = 0; (tl + 1)*font->h <= src->h; tl++) {
		for (tc = 0; (tc + 1)*font->w <= src->w; tc++) {
			aatrace_bitblt(&tilebuf[0][0], src->buf,
				       font->w, src->ll,
				       0, 0, tc*font->w, tl*font->h,
				       font->w, font->h);

			txt->buf[tl*txt->ll + tc] = aatrace_match_tile(&tilebuf[0][0], font->pic.buf,
								       font->w, font->pic.h, font->h)
				+ font->ascii_offset;
		}
	}
}

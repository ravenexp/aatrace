#include <stdlib.h>
#include "aatrace.h"

static
void aatrace_blit_tile(unsigned char* d, const unsigned char* s,
		       int tw, int ll, int ch)
{
	int l, c;

	for (l = 0; l < ch; l++)
		for (c = 0; c < tw; c++)
			d[l*tw + c] = s[l*ll + c];
}

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

void aatrace_match_pic(char* txtbuf,
		       const struct aatrace_pic* src,
		       const struct aatrace_font* font)
{
	unsigned char tilebuf[AATRACE_FONT_HEIGHT][AATRACE_FONT_WIDTH];

	int l, c;
	int tl, tc;
	int tw = 0;

	for (l = 0, tl = 0; l < src->h - font->h + 1; l += font->h, tl++) {
		for (c = 0, tc = 0; c < src->w - font->w + 1; c += font->w, tc++) {
			aatrace_blit_tile(&tilebuf[0][0], &src->buf[l*src->ll + c],
					  font->w, src->ll, font->h);

			txtbuf[tl*tw + tc] = aatrace_match_tile(&tilebuf[0][0], font->pic.buf,
								font->w, font->pic.h, font->h)
				+ font->ascii_offset;
		}

		tw = tc;
	}
}

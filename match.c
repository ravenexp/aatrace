#include <stdlib.h>
#include "aatrace.h"

static
void aatrace_blit_tile(pix_t* d, const pix_t* s,
		       int fw, int sw, int ch)
{
	int l, c;

	for (l = 0; l < ch; l++)
		for (c = 0; c < fw; c++)
			d[l*fw + c] = s[l*sw + c];
}

int aatrace_match_tile_sad(const pix_t* t, const pix_t* f,
			   int fw, int fh, int ch)
{
	int ft, cl, cc;
	int minsad = 255*fw*ch;
	int best_ft = 0;

	for (ft = 0; ft*ch < fh - ch + 1; ft++) {
		int sad = 0;

		for (cl = 0; cl < ch; cl++)
			for (cc = 0; cc < fw; cc++)
				sad += abs(t[cl*fw + cc] - f[(ft*ch + cl)*fw + cc]);

		if (sad < minsad) {
			minsad = sad;
			best_ft = ft;
		}
	}

	return best_ft;
}

int aatrace_match_tile_sadasd(const pix_t* t, const pix_t* f,
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

				d = t[cl*fw + cc] - f[(ft*ch + cl)*fw + cc];
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

void aatrace_match_pic(char* txt, const pix_t* p, const pix_t* f,
		       int w, int h, int fw, int fh, int ch)
{
	pix_t tilebuf[AATRACE_FONT_HEIGHT][AATRACE_FONT_WIDTH];

	int l, c;
	int tl, tc;
	int tw = 0;

	for (l = 0, tl = 0; l < h - ch + 1; l += ch, tl++) {
		for (c = 0, tc = 0; c < w - fw + 1; c += fw, tc++) {
			aatrace_blit_tile(&tilebuf[0][0], &p[l*w + c], fw, w, ch);

			txt[tl*tw + tc] = aatrace_match_tile(&tilebuf[0][0], f,
							     fw, fh, ch)
				+ AATRACE_FONT_ASCII_OFFSET;
		}

		tw = tc;
	}
}

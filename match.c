#include <stdlib.h>
#include "aatrace.h"

int aatrace_match_tile(const pix_t* t, const pix_t* f,
		       int w, int fw, int fh, int ch)
{
	int ft, cl, cc;
	int minsad = 255*fw*ch;
	int best_ft = 0;

	for (ft = 0; ft*ch < fh - ch + 1; ft++) {
		int sad = 0;

		for (cl = 0; cl < ch; cl++)
			for (cc = 0; cc < fw; cc++)
				sad += abs(t[cl*w + cc] - f[(ft*ch + cl)*fw + cc]);

		if (sad < minsad) {
			minsad = sad;
			best_ft = ft;
		}
	}

	return best_ft;
}

void aatrace_match_pic(char* txt, const pix_t* p, const pix_t* f,
		       int w, int h, int fw, int fh, int ch)
{
	int l, c;
	int tl, tc;
	int tw = 0;

	for (l = 0, tl = 0; l < h - ch + 1; l += ch, tl++) {
		for (c = 0, tc = 0; c < w - fw + 1; c += fw, tc++)
			txt[tl*tw + tc] = aatrace_match_tile(&p[l*w + c], f,
							     w, fw, fh, ch)
				+ AATRACE_FONT_ASCII_OFFSET;
		tw = tc;
	}
}

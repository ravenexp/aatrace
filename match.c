#include <stdlib.h>
#include "aatrace.h"
#include "bitblt.h"

struct match_result
{
	int ch;
	unsigned int score;
};

static
struct match_result
aatrace_match_tile_sad(const unsigned char* tilebuf,
		       const unsigned char* fontbuf,
		       int nchars,
		       struct aatrace_match_ctx ctx)
{
	struct match_result r;
	ctx.sad_weight = ctx.asd_weight = 0;

	const int w = AATRACE_FONT_WIDTH;
	const int h = AATRACE_FONT_HEIGHT;

	unsigned int minsad = 255*w*h;
	int best_ch = 0;

	int ch;

	for (ch = 0; ch < nchars; ch++) {
		int l, i;
		unsigned int sad = 0;

		for (l = 0; l < h; l++)
			for (i = 0; i < w; i++)
				sad += abs(  tilebuf[l*w + i]
					   - fontbuf[(ch*h + l)*w + i]);

		if (sad < minsad) {
			minsad = sad;
			best_ch = ch;
		}
	}

	r.ch = best_ch;
	r.score = minsad;
	return r;
}

static
struct match_result
aatrace_match_tile_sadasd(const unsigned char* tilebuf,
			  const unsigned char* fontbuf,
			  int nchars,
			  struct aatrace_match_ctx ctx)
{
	struct match_result r;

	const int w = AATRACE_FONT_WIDTH;
	const int h = AATRACE_FONT_HEIGHT;

	unsigned int minsadasd = (ctx.sad_weight + ctx.asd_weight)*255*w*h;
	int best_ch = 0;

	int ch;

	for (ch = 0; ch < nchars; ch++) {
		unsigned int sad = 0;
		int sd = 0;
		unsigned int asd;
		unsigned int sadasd;

		int l, i;

		for (l = 0; l < h; l++) {
			for (i = 0; i < w; i++) {
				int d = tilebuf[l*w + i]
				      - fontbuf[(ch*h + l)*w + i];
				sd += d;
				sad += abs(d);
			}
		}

		asd = abs(sd);
		sadasd = sad*ctx.sad_weight + asd*ctx.asd_weight;
		if (sadasd < minsadasd) {
			minsadasd = sadasd;
			best_ch = ch;
		}
	}

	r.ch = best_ch;
	r.score = minsadasd;
	return r;
}

static
struct match_result
aatrace_match_tile(const unsigned char* tilebuf,
		   const unsigned char* fontbuf,
		   int nchars,
		   struct aatrace_match_ctx ctx)
{
	switch (ctx.method) {
	default:
	case AATRACE_MATCH_METHOD_DEFAULT:
	case AATRACE_MATCH_METHOD_MINSAD:
		return aatrace_match_tile_sad(tilebuf, fontbuf, nchars, ctx);
	case AATRACE_MATCH_METHOD_MINSADASD:
		return aatrace_match_tile_sadasd(tilebuf, fontbuf, nchars, ctx);
	}
}

void aatrace_match_pic(struct aatrace_text* txt,
		       const struct aatrace_pic* src,
		       const struct aatrace_font* font,
		       struct aatrace_match_ctx ctx)
{
	unsigned char tilebuf[AATRACE_FONT_HEIGHT][AATRACE_FONT_WIDTH];

	int tl, tc;

	txt->ll = txt->w = src->w/AATRACE_FONT_WIDTH;
	txt->h = src->h/AATRACE_FONT_HEIGHT;
	txt->buf = (char*)malloc(txt->ll*txt->h);

	if (!ctx.sad_weight)
		ctx.sad_weight = AATRACE_DEFAULT_SAD_WEIGHT;
	if (!ctx.asd_weight)
		ctx.asd_weight = AATRACE_DEFAULT_ASD_WEIGHT;

	for (tl = 0; (tl + 1)*font->h <= src->h; tl++) {
		for (tc = 0; (tc + 1)*font->w <= src->w; tc++) {
			struct match_result r;

			aatrace_bitblt(&tilebuf[0][0], src->buf,
				       font->w, src->ll,
				       0, 0, tc*font->w, tl*font->h,
				       font->w, font->h);

			r = aatrace_match_tile(&tilebuf[0][0], font->pic.buf,
					       font->nchars, ctx);

			txt->buf[tl*txt->ll + tc] = r.ch + font->ascii_offset;
		}
	}
}

#include <stdlib.h>
#include <string.h>
#include "aatrace.h"

static
void aatrace_diff2(unsigned char* d, unsigned char* s,
		   int w, int h,
		   unsigned int scale,
		   unsigned int threshold)
{
	int l, i;
	unsigned int D;

	for (l = 0; l < h - 1; l++)
		for (i = l*w; i < (l + 1)*w - 1; i++) {
			D = abs(s[i + w] - s[i] + s[i + w + 1] - s[i + 1])
			  + abs(s[i + 1] - s[i] + s[i + w + 1] - s[i + w]);

			D = (D*scale)/4;
			D = (D > 255) ? 255 : D;
			d[i] = (D >= threshold) ? D : 0;
		}
}

static
void aatrace_diff3(unsigned char* d, unsigned char* s,
		   int w, int h,
		   unsigned int scale,
		   unsigned int threshold)
{
	int l, i;
	unsigned int D;

	for (l = 1; l < h - 1; l++)
		for (i = l*w + 1; i < (l + 1)*w - 1; i++) {
			D = abs(2*(s[i + w + 0] - s[i - w + 0])
				+  s[i + w - 1] - s[i - w - 1]
				+  s[i + w + 1] - s[i - w + 1])
			    +
			    abs(2*(s[i + 0 + 1] - s[i + 0 - 1])
				+  s[i - w + 1] - s[i - w - 1]
				+  s[i + w + 1] - s[i + w - 1]);

			D = (D*scale)/8;
			D = (D > 255) ? 255 : D;
			d[i] = (D >= threshold) ? D : 0;
		}
}

static const int diff4_Kx[4][4] =
{
	{-1, -1,  1,  1},
	{-2, -2,  2,  2},
	{-2, -2,  2,  2},
	{-1, -1,  1,  1}
};

static const int diff4_Ky[4][4] =
{
	{-1, -2, -2, -1},
	{-1, -2, -2, -1},
	{ 1,  2,  2,  1},
	{ 1,  2,  2,  1}
};

static
void aatrace_diff4(unsigned char* d, unsigned char* s,
		   int w, int h,
		   unsigned int scale,
		   unsigned int threshold)
{
	int l, i;
	int kl, kc;
	unsigned int D;

	for (l = 1; l < h - 2; l++)
		for (i = l*w + 1; i < (l + 1)*w - 2; i++) {
			int Dx = 0, Dy = 0;

			for (kl = 0; kl < 4; kl++)
				for (kc = 0; kc < 4; kc++) {
					Dx += s[i + (kl - 1)*w + kc - 1]
						* diff4_Kx[kl][kc];
					Dy += s[i + (kl - 1)*w + kc - 1]
						* diff4_Ky[kl][kc];
			}

			D = abs(Dx) + abs(Dy);
			D = (D*scale)/24;
			D = (D > 255) ? 255 : D;
			d[i] = (D >= threshold) ? D : 0;
		}
}


void aatrace_diff_ctx_init(struct aatrace_diff_ctx* ctx)
{
	ctx->scale = AATRACE_DIFF_SCALE_DEFAULT;
	ctx->kernel = AATRACE_DEFAULT_DIFF_KERNEL;
	ctx->threshold = AATRACE_DIFF_THRESHOLD_DEFAULT;
}

void aatrace_diff(struct aatrace_pic* dst,
		  const struct aatrace_pic* src,
		  const struct aatrace_diff_ctx* ctx)
{
	dst->w = src->w;
	dst->h = src->h;
	dst->ll = src->ll;

	dst->buf = (unsigned char*)malloc(dst->ll*dst->h);

	switch (ctx->kernel) {
	default:
	case AATRACE_DIFF_KERNEL_NONE:
		memcpy(dst->buf, src->buf, src->ll*src->h);
		break;
	case AATRACE_DIFF_KERNEL_2x2:
		aatrace_diff2(dst->buf, src->buf, src->ll, src->h,
			      ctx->scale, ctx->threshold);
		break;
	case AATRACE_DIFF_KERNEL_3x3:
		aatrace_diff3(dst->buf, src->buf, src->ll, src->h,
			      ctx->scale, ctx->threshold);
		break;
	case AATRACE_DIFF_KERNEL_4x4:
		aatrace_diff4(dst->buf, src->buf, src->ll, src->h,
			      ctx->scale, ctx->threshold);
		break;
	}
}

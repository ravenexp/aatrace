#include <stdlib.h>
#include "aatrace.h"

void aatrace_diff2(pix_t* d, pix_t* s, int w, int h)
{
	int l, i;

	for (l = 0; l < h - 1; l++)
		for (i = l*w; i < (l + 1)*w - 1; i++) {
			int D = abs(s[i + w] - s[i] + s[i + w + 1] - s[i + 1])
			      + abs(s[i + 1] - s[i] + s[i + w + 1] - s[i + w]);

			D = (D*AATRACE_DIFF_SCALE)/4;
			d[i] = (D > 255) ? 255 : D;
		}
}

void aatrace_diff3(pix_t* d, pix_t* s, int w, int h)
{
	int l, i;

	for (l = 1; l < h - 1; l++)
		for (i = l*w + 1; i < (l + 1)*w - 1; i++) {
			int D = abs(2*(s[i + w + 0] - s[i - w + 0])
				    +  s[i + w - 1] - s[i - w - 1]
				    +  s[i + w + 1] - s[i - w + 1])
				+
				abs(2*(s[i + 0 + 1] - s[i + 0 - 1])
				    +  s[i - w + 1] - s[i - w - 1]
				    +  s[i + w + 1] - s[i + w - 1]);

			D = (D*AATRACE_DIFF_SCALE)/8;
			d[i] = (D > 255) ? 255 : D;
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

void aatrace_diff4(pix_t* d, pix_t* s, int w, int h)
{
	int l, i;
	int kl, kc;

	for (l = 1; l < h - 2; l++)
		for (i = l*w + 1; i < (l + 1)*w - 2; i++) {
			int Dx = 0, Dy = 0;
			int D;

			for (kl = 0; kl < 4; kl++)
				for (kc = 0; kc < 4; kc++) {
					Dx += s[i + kl*w + kc]*diff4_Kx[kl][kc];
					Dy += s[i + kl*w + kc]*diff4_Ky[kl][kc];
			}

			D = abs(Dx) + abs(Dy);

			D = (D*AATRACE_DIFF_SCALE)/24;
			d[i] = (D > 255) ? 255 : D;
		}
}

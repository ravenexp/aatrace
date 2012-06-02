#include <stdlib.h>
#include "aatrace.h"

static
void aatrace_diff2(pix_t* d, pix_t* s, int w, int h,
		   unsigned int scale)
{
	int l, i;
	unsigned int D;

	for (l = 0; l < h - 1; l++)
		for (i = l*w; i < (l + 1)*w - 1; i++) {
			D = abs(s[i + w] - s[i] + s[i + w + 1] - s[i + 1])
			  + abs(s[i + 1] - s[i] + s[i + w + 1] - s[i + w]);

			D = (D*scale)/4;
			d[i] = (D > 255) ? 255 : D;
		}
}

static
void aatrace_diff3(pix_t* d, pix_t* s, int w, int h,
		   unsigned int scale)
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

static
void aatrace_diff4(pix_t* d, pix_t* s, int w, int h,
		   unsigned int scale)
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
			d[i] = (D > 255) ? 255 : D;
		}
}

void aatrace_diff(pix_t* d, pix_t* s, int w, int h,
		  unsigned int scale,
		  int kernel)
{
	switch (kernel) {
	default:
	case 2:
		return aatrace_diff2(d, s, w, h, scale);
	case 3:
		return aatrace_diff3(d, s, w, h, scale);
	case 4:
		return aatrace_diff4(d, s, w, h, scale);
	}
}

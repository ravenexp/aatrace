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

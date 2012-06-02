#include <stdio.h>
#include <stdlib.h>

#include "pnm.h"
#include "aatrace.h"
#include "util.h"

int main(int argc, char* argv[])
{
	pix_t* sbuf;
	pix_t* dbuf;
	int width, height;
	unsigned int scale = 0;
	int kernel = 0;

	if (argc < 3)
		return 1;

	sbuf = util_load_pic(argv[1], &width, &height);
	if (!sbuf)
		return 2;

	if (argc > 3)
		scale = atoi(argv[3]);
	if (!scale)
		scale = AATRACE_DIFF_SCALE_DEFAULT;

	if (argc > 4)
		kernel = atoi(argv[4]);
	if (!kernel)
		kernel = AATRACE_DIFF_KERNEL_DEFAULT;

	dbuf = (pix_t*)malloc(width*height);
	aatrace_diff(dbuf, sbuf, width, height, scale, kernel);
	util_store_pic(argv[2], dbuf, width, height);

	return 0;
}

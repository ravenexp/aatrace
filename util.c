#include <stdio.h>
#include <stdlib.h>
#include "aatrace.h"
#include "pnm.h"

int util_dump_text_file(FILE* f, const char* txt,
			int twidth, int theight)
{
	int l;

	for (l = 0; l < theight; l++) {
		if (fwrite(&txt[l*twidth], twidth, 1, f) != 1)
			return -1;

		putc('\n', f);
	}

	return (twidth + 1)*theight;
}


int util_dump_text_filename(const char* fname, const char* txt,
			    int twidth, int theight)
{
	FILE* f;

	f = fopen(fname, "wb");
	if (!f)
		return -1;

	return util_dump_text_file(f, txt, twidth, theight);
}

pix_t* util_load_pic(const char* fname, int* pwidth, int* pheight)
{
	pix_t* picbuf;
	pnm_file pnm;
	pnm_type_t type;

	pnm = pnm_filename_open(fname, &type, pwidth, pheight);
	if (!pnm || type != PNM_TYPE_PGM)
		return 0;

	picbuf = (pix_t*)malloc(*pwidth**pheight);

	if (pnm_file_read_pic(pnm, picbuf) < 0)
		return 0;

	pnm_file_close(pnm);

	return picbuf;
}

int util_store_pic(const char* fname, const pix_t* picbuf,
		   int width, int height)
{
	pnm_file pnm;
	int written;

	pnm = pnm_filename_create(fname, PNM_TYPE_PGM, width, height);
	if (!pnm)
		return -1;

	written = pnm_file_write_pic(pnm, picbuf);
	pnm_file_close(pnm);

	return written;
}

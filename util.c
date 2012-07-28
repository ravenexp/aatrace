#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "aatrace.h"
#include "pnm.h"

char* util_read_text(FILE* f, int* ptwidth, int* ptheight)
{
	size_t sz = 0x1000;
	char* txt = (char*)malloc(sz);
	unsigned int w, l;

	if (!fgets(txt, sz, f))
		return 0;

	w = strlen(txt) - 1;

	for (l = 1; ; l++) {
		if ((l + 1)*w + 1 > sz) {
			sz *= 2;
			txt = (char*)realloc(txt, sz);
		}

		if (!fgets(&txt[l*w], sz - l*w, f))
			break;
	}

	*ptwidth = w;
	*ptheight = l;

	return txt;
}

char* util_load_text(const char* fname, int* ptwidth, int* ptheight)
{
	FILE* f = stdin;

	if (strcmp(fname, "-")) {
		f = fopen(fname, "rb");
		if (!f)
			return 0;
	}

	return util_read_text(f, ptwidth, ptheight);
}

int util_write_text(FILE* f, const char* txt, int twidth, int theight)
{
	int l;

	for (l = 0; l < theight; l++) {
		if (fwrite(&txt[l*twidth], twidth, 1, f) != 1)
			return -1;

		putc('\n', f);
	}

	return (twidth + 1)*theight;
}

int util_store_text(const char* fname, const char* txt,
		    int twidth, int theight)
{
	FILE* f = stdout;

	if (strcmp(fname, "-")) {
		f = fopen(fname, "wb");
		if (!f)
			return -1;
	}

	return util_write_text(f, txt, twidth, theight);
}

int util_load_pic(struct aatrace_pic* pic, const char* fname)
{
	pnm_file pnm;
	pnm_type_t type;

	if (strcmp(fname, "-")) {
		pnm = pnm_filename_open(fname, &type, &pic->w, &pic->h);
	} else {
		pnm = pnm_file_open(stdin, &type, &pic->w, &pic->h);
	}

	if (!pnm || type != PNM_TYPE_PGM)
		return -1;

	pic->ll = pic->w;
	pic->buf = (unsigned char*)malloc(pic->ll*pic->h);

	if (pnm_file_read_pic(pnm, pic->buf) < 0)
		return -1;

	pnm_file_close(pnm);

	return 0;
}

int util_store_pic(const char* fname, const struct aatrace_pic* pic)
{
	pnm_file pnm;
	int written;

	if (strcmp(fname, "-")) {
		pnm = pnm_filename_create(fname, PNM_TYPE_PGM, pic->w, pic->h);
	} else {
		pnm = pnm_file_create(stdout, PNM_TYPE_PGM, pic->w, pic->h);
	}

	if (!pnm)
		return -1;

	written = pnm_file_write_pic(pnm, pic->buf);

	pnm_file_close(pnm);

	return written;
}

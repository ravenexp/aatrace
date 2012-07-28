#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "aatrace.h"
#include "pnm.h"
#include "util.h"

int util_read_text(struct aatrace_text* txt, FILE* f)
{
	size_t sz = 0x1000;
	txt->buf = (char*)malloc(sz);
	unsigned int l;

	if (!fgets(txt->buf, sz, f))
		return -1;

	txt->ll = txt->w = strlen(txt->buf) - 1;

	for (l = 1; ; l++) {
		if ((l + 1)*txt->ll + 1 > sz) {
			sz *= 2;
			txt->buf = (char*)realloc(txt->buf, sz);
		}

		if (!fgets(&txt->buf[l*txt->ll], sz - l*txt->ll, f))
			break;
	}

	txt->h = l;

	return 0;
}

int util_load_text(struct aatrace_text* txt, const char* fname)
{
	FILE* f = stdin;

	if (strcmp(fname, "-")) {
		f = fopen(fname, "rb");
		if (!f)
			return 0;
	}

	return util_read_text(txt, f);
}

int util_write_text(FILE* f, const struct aatrace_text* txt)
{
	int l;

	for (l = 0; l < txt->h; l++) {
		if (fwrite(&txt->buf[l*txt->ll], txt->w, 1, f) != 1)
			return -1;

		putc('\n', f);
	}

	return (txt->w + 1)*txt->h;
}

int util_store_text(const char* fname, const struct aatrace_text* txt)
{
	FILE* f = stdout;

	if (strcmp(fname, "-")) {
		f = fopen(fname, "wb");
		if (!f)
			return -1;
	}

	return util_write_text(f, txt);
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

int util_load_font(struct aatrace_font* font, const char* fname)
{
	if (util_load_pic(&font->pic, fname) < 0)
		return -1;

	if (font->pic.w != AATRACE_FONT_WIDTH)
		return -1;

	font->w = AATRACE_FONT_WIDTH;
	font->h = AATRACE_FONT_HEIGHT;
	font->nchars = font->pic.h/AATRACE_FONT_HEIGHT;
	font->ascii_offset = AATRACE_FONT_ASCII_OFFSET;

	return 0;
}

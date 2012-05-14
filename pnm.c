#include <stdio.h>
#include <stdlib.h>

#include "pnm.h"

struct pnm_file_s
{
	FILE* file;
	int type;
	int width, height;
	int stride;
};

pnm_file pnm_filename_open(const char* fname, pnm_type_t* ptype, int* pwidth, int* pheight)
{
	FILE* f;
	pnm_file pnm;

	f = fopen(fname, "rb");
	if (!f)
		return 0;

	pnm = pnm_file_open(f, ptype, pwidth, pheight);
	if (!pnm) {
		fclose(f);
		return 0;
	}

	return pnm;
}

pnm_file pnm_file_open(FILE* f, pnm_type_t* ptype, int* pwidth, int* pheight)
{
	char magic, typec;
	int depth;
	int i;
	pnm_file s;

	if (fscanf(f, "%c%c\n", &magic, &typec) != 2)
		return 0;

	if (magic != 'P' || (typec != '5' && typec != '6'))
		return 0;

	*ptype = (pnm_type_t)(typec - '0');

	/* comments */
	for (i = 0; i < 16; i++)
		if (!fscanf(f, "#%*[^\n]\n"))
			break;

	if (fscanf(f, "%d %d\n", pwidth, pheight) != 2)
		return 0;

	if (fscanf(f, "%d\n", &depth) != 1)
		return 0;

	if (depth != 255)
		return 0;

	s = (pnm_file)malloc(sizeof(*s));
	s->file = f;
	s->type = *ptype;
	s->width = *pwidth;
	s->height = *pheight;
	s->stride = ((s->type == 6) ? 3 : 1) * s->width;

	return s;
}

int pnm_file_read_line(pnm_file s, pnm_pix_t* linebuf)
{
	if (fread(linebuf, s->stride, 1, s->file) != 1)
		return -1;

	return s->stride;
}

int pnm_file_read_pic(pnm_file s, pnm_pix_t* picbuf)
{
	int l;

	for (l = 0; l < s->height; l++)
		if (pnm_file_read_line(s, picbuf + l*s->stride) < 0)
			return -1;

	return s->stride*s->height;
}

pnm_file pnm_filename_create(const char* fname, pnm_type_t type, int width, int height)
{
	FILE* f;
	pnm_file pnm;

	f = fopen(fname, "wb");
	if (!f)
		return 0;

	pnm = pnm_file_create(f, type, width, height);
	if (!pnm) {
		fclose(f);
		return 0;
	}

	return pnm;
}

pnm_file pnm_file_create(FILE* f, pnm_type_t type, int width, int height)
{
	pnm_file s;

	s = (pnm_file)malloc(sizeof(*s));

	s->file = f;
	s->type = type;
	s->width = width;
	s->height = height;
	s->stride = ((s->type == 6) ? 3 : 1) * s->width;

	fprintf(f, "P%d\n%d %d\n255\n", type, width, height);

	return s;
}

int pnm_file_write_line(pnm_file s, const pnm_pix_t* linebuf)
{
	if (fwrite(linebuf, s->stride, 1, s->file) != 1)
		return -1;

	return s->stride;
}

int pnm_file_write_pic(pnm_file s, const pnm_pix_t* picbuf)
{
	int l;

	for (l = 0; l < s->height; l++)
		if (pnm_file_write_line(s, picbuf + l*s->stride) < 0)
			return -1;

	return s->stride*s->height;
}

int pnm_file_release(pnm_file s)
{
	free(s);

	return 0;
}

int pnm_file_close(pnm_file s)
{
	fclose(s->file);
	free(s);

	return 0;
}


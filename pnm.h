struct pnm_file_s;
typedef struct pnm_file_s* pnm_file;

typedef enum {
	PNM_TYPE_UNSUPPORTED,
	PNM_TYPE_PGM = 5,
	PNM_TYPE_PPM = 6
} pnm_type_t;

typedef unsigned char pnm_pix_t;

pnm_file pnm_file_open(FILE* f, pnm_type_t* ptype, int* pwidth, int* pheight);
pnm_file pnm_file_create(FILE* f, pnm_type_t type, int width, int height);

pnm_file pnm_filename_open(const char* fname, pnm_type_t* ptype,
			   int* pwidth, int* pheight);
pnm_file pnm_filename_create(const char* fname, pnm_type_t type,
			     int width, int height);

int pnm_file_read_line(pnm_file, pnm_pix_t* linebuf);
int pnm_file_write_line(pnm_file, const pnm_pix_t* linebuf);

int pnm_file_read_pic(pnm_file, pnm_pix_t* picbuf);
int pnm_file_write_pic(pnm_file, const pnm_pix_t* picbuf);

int pnm_file_close(pnm_file);
int pnm_file_release(pnm_file);

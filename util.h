char* util_read_text(FILE* f, int* ptwidth, int* ptheight);
char* util_load_text(const char* fname, int* ptwidth, int* ptheight);

int util_write_text(FILE* f, const char* txt, int twidth, int theight);
int util_store_text(const char* fname, const char* txt,
		    int twidth, int theight);

pix_t* util_load_pic(const char* fname, int* pwidth, int* pheight);
int util_store_pic(const char* fname, const pix_t* picbuf,
		   int width, int height);

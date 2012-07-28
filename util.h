char* util_read_text(FILE* f, int* ptwidth, int* ptheight);
char* util_load_text(const char* fname, int* ptwidth, int* ptheight);

int util_write_text(FILE* f, const char* txt, int twidth, int theight);
int util_store_text(const char* fname, const char* txt,
		    int twidth, int theight);

int util_load_pic(struct aatrace_pic*, const char* fname);
int util_store_pic(const char* fname, const struct aatrace_pic*);

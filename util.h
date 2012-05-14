int util_dump_text_file(FILE* f, const char* txt,
			int twidth, int theight);
int util_dump_text_filename(const char* fname, const char* txt,
			    int twidth, int theight);

pix_t* util_load_pic(const char* fname,
		     int* pwidth, int* pheight);
int util_store_pic(const char* fname, const pix_t* picbuf,
		   int width, int height);

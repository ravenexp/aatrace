int util_read_text(struct aatrace_text* txt, FILE* f);
int util_load_text(struct aatrace_text* txt, const char* fname);
int util_write_text(FILE* f, const struct aatrace_text* txt);
int util_store_text(const char* fname, const struct aatrace_text* txt);

int util_load_pic(struct aatrace_pic*, const char* fname);
int util_store_pic(const char* fname, const struct aatrace_pic*);

int util_load_font(struct aatrace_font*, const char* fname);

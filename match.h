/* Match code assumes FW == font->w, FH == font->h */

#define FW AATRACE_FONT_WIDTH
#define FH AATRACE_FONT_HEIGHT

struct match_result
{
	int ch;
	unsigned int score;
};

void aatrace_match_ctx_init(struct aatrace_match_ctx* ctx);

struct match_result
aatrace_match_tile(const unsigned char* tilebuf,
		   const unsigned char* fontbuf,
		   int nchars,
		   const struct aatrace_match_ctx* ctx);

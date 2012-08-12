#define AATRACE_FONT_WIDTH  8
#define AATRACE_FONT_HEIGHT 16

/* ASCII code of the first font character */
#define AATRACE_FONT_ASCII_OFFSET 32

/* Gradiant map scale adjustment
   1 ::= normalized to the byte range
*/
#define AATRACE_DIFF_SCALE_DEFAULT 4

/* Differentiation kernel implementation */
enum aatrace_diff_kernel {
	AATRACE_DIFF_KERNEL_DEFAULT,
	AATRACE_DIFF_KERNEL_NONE,
	AATRACE_DIFF_KERNEL_2x2,
	AATRACE_DIFF_KERNEL_3x3,
	AATRACE_DIFF_KERNEL_4x4
};

#define AATRACE_DEFAULT_DIFF_KERNEL AATRACE_DIFF_KERNEL_3x3

/* Image tile to font characters matching method */
enum aatrace_match_method {
	AATRACE_MATCH_METHOD_DEFAULT,
	AATRACE_MATCH_METHOD_MINSAD,
	AATRACE_MATCH_METHOD_MINSADASD
};

#define AATRACE_DEFAULT_MATCH_METHOD AATRACE_MATCH_METHOD_MINSADASD

#define AATRACE_DEFAULT_SAD_WEIGHT 5
#define AATRACE_DEFAULT_ASD_WEIGHT 2

enum aatrace_search_method {
	AATRACE_SEARCH_METHOD_DEFAULT,
	AATRACE_SEARCH_METHOD_NONE,
	AATRACE_SEARCH_METHOD_DIAMOND,
	AATRACE_SEARCH_METHOD_BOX
};

#define AATRACE_DEFAULT_SEARCH_METHOD AATRACE_SEARCH_METHOD_NONE

#define AATRACE_SEARCH_FLAG_COVERAGE 1

struct aatrace_pic
{
	unsigned char* buf;
	unsigned int ll;
	int w, h;
};

struct aatrace_font
{
	struct aatrace_pic pic;
	int w, h;
	int nchars;
	int ascii_offset;
};

struct aatrace_text
{
	char* buf;
	unsigned int ll;
	int w, h;
};

struct aatrace_diff_ctx
{
	enum aatrace_diff_kernel kernel;
	unsigned int scale;
};

struct aatrace_match_ctx
{
	enum aatrace_match_method method;
	unsigned int sad_weight, asd_weight;
};

struct aatrace_search_ctx
{
	enum aatrace_search_method method;
	unsigned int flags;
	unsigned int range;
};

struct aatrace_convert_ctx
{
	struct aatrace_match_ctx match;
	struct aatrace_search_ctx search;
};

void aatrace_diff(struct aatrace_pic* dst, const struct aatrace_pic* src,
		  struct aatrace_diff_ctx ctx);

void aatrace_convert_pic(struct aatrace_text* txt,
			 const struct aatrace_pic* src,
			 const struct aatrace_font* font,
			 struct aatrace_convert_ctx ctx);

void aatrace_render_pic(struct aatrace_pic* out,
			const struct aatrace_text* txt,
			const struct aatrace_font* font);

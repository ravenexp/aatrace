#define AATRACE_FONT_WIDTH  8
#define AATRACE_FONT_HEIGHT 16

/* ASCII code of the first font character */
#define AATRACE_FONT_ASCII_OFFSET 32

/* Gradiant map scale adjustment
   1 ::= normalized to the byte range
*/
#define AATRACE_DIFF_SCALE_DEFAULT 4

/* Differentiation kernel implementation
   2 - 2x2 flat top
   3 - 3x3 Sobel operator
   4 - 4x4 [1 2 2 1]^2 operator
 */
enum aatrace_diff_kernel {
	AATRACE_DIFF_KERNEL_DEFAULT,
	AATRACE_DIFF_KERNEL_NONE,
	AATRACE_DIFF_KERNEL_2x2,
	AATRACE_DIFF_KERNEL_3x3,
	AATRACE_DIFF_KERNEL_4x4
};

#define AATRACE_DEFAULT_DIFF_KERNEL AATRACE_DIFF_KERNEL_3x3


#define aatrace_match_tile aatrace_match_tile_sad
/*
#define aatrace_match_tile aatrace_match_tile_sadasd
*/

struct aatrace_pic
{
	unsigned char* buf;
	unsigned int ll;
	int w, h;
};

struct aatrace_diff_ctx
{
	unsigned int scale;
	enum aatrace_diff_kernel kernel;
};

void aatrace_diff(struct aatrace_pic* dst, const struct aatrace_pic* src,
		  struct aatrace_diff_ctx ctx);

void aatrace_match_pic(char* txtbuf,
		       const struct aatrace_pic* src,
		       const struct aatrace_pic* font,
		       int char_h);

void aatrace_render_pic(struct aatrace_pic* out,
			const char* txt,
			const struct aatrace_pic* font,
			int tw, int th, int char_h);

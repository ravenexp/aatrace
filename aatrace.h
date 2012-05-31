#define AATRACE_FONT_WIDTH  8
#define AATRACE_FONT_HEIGHT 16

/* ASCII code of the first font character */
#define AATRACE_FONT_ASCII_OFFSET 32

/* Gradiant map scale adjustment
   1 ::= normalized to the byte range
*/
#define AATRACE_DIFF_SCALE_DEFAULT 4

/* Differentiation kernel implementation
   diff2 - 2x2 flat top
   diff3 - 3x3 Sobel operator
   diff4 - 4x4 [1 2 2 1]^2 operator
 */
#define aatrace_diff aatrace_diff3

typedef unsigned char pix_t;

void aatrace_diff2(pix_t* d, pix_t* s, int w, int h,
		   unsigned int scale);
void aatrace_diff3(pix_t* d, pix_t* s, int w, int h,
		   unsigned int scale);
void aatrace_diff4(pix_t* d, pix_t* s, int w, int h,
		   unsigned int scale);

int aatrace_match_tile(const pix_t* t, const pix_t* f,
		       int w, int fw, int fh, int ch);
void aatrace_match_pic(char* txtbuf, const pix_t* p, const pix_t* f,
		       int w, int h, int fw, int fh, int ch);

void aatrace_render_tile(pix_t* t, const pix_t* f, int tl,
			 int w, int fw, int ch);
void aatrace_render_pic(pix_t* p, const char* txt, const pix_t* f,
			int w, int tw, int th, int fw, int ch);

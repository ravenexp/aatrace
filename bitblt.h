static
void aatrace_bitblt(unsigned char* d, const unsigned char* s,
		    unsigned int dll, unsigned int sll,
		    int dx, int dy, int sx, int sy,
		    int w, int h)
{
	int l, i;

	for (l = 0; l < h; l++)
		for (i = 0; i < w; i++)
			d[(l + dy)*dll + dx + i] = s[(l + sy)*sll + sx + i];
}

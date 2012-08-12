/* Calculates score penalty for offset tiles */
unsigned int
aatrace_cover_tile_score(const struct aatrace_pic* pic,
			 const struct aatrace_font* font,
			 int tl, int tc, int dx, int dy,
			 struct aatrace_match_ctx ctx);

/* Returns the best font character number found */
int aatrace_search_tile(const struct aatrace_pic* src,
			const struct aatrace_font* font,
			int tl, int tc,
			struct aatrace_convert_ctx ctx);

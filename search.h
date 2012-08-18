void aatrace_search_ctx_init(struct aatrace_search_ctx* ctx);

/* Calculates score penalty for offset tiles */
unsigned int
aatrace_cover_tile_score(const struct aatrace_pic* pic,
			 const struct aatrace_font* font,
			 int tl, int tc, int dx, int dy,
			 const struct aatrace_match_ctx* ctx);

/* Returns the best font character number found */
int aatrace_search_tile(const struct aatrace_pic* src,
			const struct aatrace_font* font,
			int tl, int tc,
			const struct aatrace_match_ctx* m_ctx,
			const struct aatrace_search_ctx* ctx);

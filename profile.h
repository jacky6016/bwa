#include <stdint.h>

typedef struct {
    uint8_t n;
    uint32_t size;
    uint64_t* index;
    uint64_t* content;
} n_way_cache_t; 

typedef struct {
	int num_seeds = 0, num_chains = 0; 
	int p1_extend = 0, p2_extend = 0, p3_extend = 0;
	int inv_psi = 0;
} profile_per_read_t; 

typedef struct {
    uint64_t total_req_bwt;
    uint64_t total_req_sa;
    uint64_t num_hits_bwt;
    uint64_t num_hits_sa;

	// caches
    n_way_cache_t occ_lookup_cache;
    n_way_cache_t sa_lookup_cache;
} profile_total_t;
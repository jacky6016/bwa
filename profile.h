#include <stdint.h>
#include <stddef.h>

typedef struct {
    uint8_t n;
    size_t size;
    uint8_t * capacity;
    uint64_t* content;
} n_way_cache_t; 

typedef struct {
	uint32_t num_seeds;
    uint32_t num_chains; 
	uint32_t p1_extend, p2_extend, p3_extend;
	uint32_t inv_psi;
    uint32_t sa_lookup;
} profile_per_read_t; 

typedef struct {
    uint64_t total_req_bwt;
    uint64_t total_req_sa;
    uint64_t num_hits_bwt;
    uint64_t num_hits_sa;

	// caches
    n_way_cache_t* occ_lookup_cache;
    n_way_cache_t* sa_lookup_cache;
} profile_total_t;

void cache_lookup (n_way_cache_t *cache, uint64_t value);

void cache_insert (n_way_cache_t *cache, uint64_t value);

uint64_t hash (uint64_t value);
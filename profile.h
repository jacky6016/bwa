#ifndef PROFILE_H_
#define PROFILE_H_

#include <stdint.h>
#include <stddef.h>

typedef struct {
    size_t n;
    size_t size;
    size_t *capacity;
    size_t *rr_ptr;
    uint64_t* content;  //dram addresses
} n_way_cache_t;

typedef struct {
    uint32_t num_seeds;
    uint32_t num_chains;
    uint32_t p1_extend, p2_extend, p3_extend;
    uint32_t inv_psi;
} profile_per_read_t;

typedef struct {
    uint64_t total_req_bwt;
    uint64_t total_req_sa;
    uint64_t num_hits_bwt;
    uint64_t num_hits_sa;

    // N-way caches (haven't implemented LRU mechanism yet)
    n_way_cache_t* occ_lookup_cache;
    n_way_cache_t* sa_lookup_cache;
} profile_total_t;

n_way_cache_t *cache_init ();

void cache_destroy (n_way_cache_t *cache);

int cache_lookup (n_way_cache_t *cache, uint64_t value);

void cache_insert (n_way_cache_t *cache, uint64_t value);

uint64_t hash (uint64_t value);

void per_read_to_file (char *filename, profile_per_read_t* read_profile);

profile_total_t *profile_total_init (size_t n, size_t size);

void profile_total_destroy (profile_total_t *total_profile);

void total_to_file (char *filename, profile_total_t *total_profile);

#endif
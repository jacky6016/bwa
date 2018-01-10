#include <stdint.h>
#include <stddef.h>
#include <unistd.h>
#include <stdio.h>
#include <inttypes.h>
#include <stdlib.h>  // for malloc()
#include <string.h>  // for memset()
#include "profile.h"

#define BIT_MASK_32  ((1LL << 32) - 1)

n_way_cache_t *cache_init (size_t n, size_t size)
{
    n_way_cache_t *tmp = (n_way_cache_t *)malloc(sizeof(n_way_cache_t));
    memset(tmp, 0, sizeof(n_way_cache_t));

    tmp->n = n;
    tmp->size = size;

    tmp->capacity = (size_t *)malloc(sizeof(size_t) * size);
    memset(tmp->capacity, 0, sizeof(sizeof(size_t) * size));

    tmp->rr_ptr = (size_t *)malloc(sizeof(size_t) * size);
    memset(tmp->rr_ptr, 0, sizeof(sizeof(size_t) * size));

    tmp->content = (uint64_t *)malloc(sizeof(uint64_t) * size * n);
    memset(tmp->content, 0, sizeof(sizeof(uint64_t) * size * n));

    return tmp;
}

void cache_destroy (n_way_cache_t *cache)
{
    free(cache->capacity);
    free(cache->rr_ptr);
    free(cache->content);
    free(cache);
}

int cache_lookup (n_way_cache_t *cache, uint64_t value)
{
    int i;
    uint64_t index = hash(value) % cache->size;

    for (i = 0; i < cache->n; i++)
        if (cache->content[index * cache->n + i] == value)
            return 1; // cache hit

    return 0; // cache miss
}

void cache_insert (n_way_cache_t *cache, uint64_t value)
{
    uint64_t index = hash(value) % cache->size;
    size_t insert_pos = index * cache->n + (cache->rr_ptr[index]);
    cache->content[insert_pos] = value;
    cache->rr_ptr[index] = (cache->rr_ptr[index] + 1) % cache->n;
    cache->capacity[index] = (cache->capacity[index] < cache->n) ? (cache->capacity[index] + 1) : cache->n;
}

uint64_t hash (uint64_t value)
{
    return (value * (uint64_t)2654435761) & BIT_MASK_32;
}

void per_read_to_file (char *filename, profile_per_read_t* read_profile)
{
    FILE *fp;
    if (access(filename, F_OK) != -1) {
        // file exists
        fp = fopen(filename, "a");
        fprintf(fp, "%d,%d,%d,%d,%d,%d,", read_profile->p1_extend, read_profile->p2_extend, read_profile->p3_extend, read_profile->inv_psi, read_profile->num_seeds, read_profile->num_chains);
        fprintf(fp, "%lf,", (double)read_profile->extend_time / (double)(read_profile->p1_extend + read_profile->p2_extend + read_profile->p3_extend));
        fprintf(fp, "%lf,", (double)read_profile->sa_lookup_time / (double)(read_profile->num_seeds));
        fprintf(fp, "%lf\n", (double)read_profile->chain_time / (double)(read_profile->num_chains));
    } else {
        fp = fopen(filename, "w");
        fprintf(fp, "p1_extend,p2_extend,p3_extend,inv_psi,num_seeds,num_chains,extend_time,sa_lookup_time,chain_time\n");
    }
    fclose(fp);
}

profile_total_t *profile_total_init (size_t n, size_t size)
{
    profile_total_t *tmp = (profile_total_t *)malloc(sizeof(profile_total_t));
    memset(tmp, 0, sizeof(profile_total_t));
    tmp->occ_lookup_cache = cache_init(n, size);
    tmp->sa_lookup_cache = cache_init(n, size);

    return tmp;
}

void profile_total_destroy (profile_total_t *total_profile)
{
    cache_destroy(total_profile->occ_lookup_cache);
    cache_destroy(total_profile->sa_lookup_cache);
    free(total_profile);
}

void total_to_file (char *filename, profile_total_t *total_profile)
{
    FILE *fp;
    fp = fopen(filename, "w");
    fprintf(fp, "Extension Cache: hits = %" PRIu64 ", total requests = %" PRIu64, total_profile->num_hits_bwt, total_profile->total_req_bwt);
    if (total_profile->total_req_bwt > 0)
        fprintf(fp, ", hit rate = %lf\n", (double)total_profile->num_hits_bwt / (double)total_profile->total_req_bwt);
    fprintf(fp, "\nSA Cache: hits = %" PRIu64 ", total requests = %" PRIu64, total_profile->num_hits_sa, total_profile->total_req_sa);
    if (total_profile->total_req_sa > 0)
        fprintf(fp, ", hit rate = %lf\n", (double)total_profile->num_hits_sa / (double)total_profile->total_req_sa);
    
    fclose(fp);
}

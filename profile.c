#include <stdint.h>
#include <stddef.h>
#include <unistd.h>
#include <stdio.h>
#include "profile.h"

#define BIT_MASK_32  ((1LL << 32) - 1)

int cache_lookup (n_way_cache_t *cache, uint64_t value)
{
    int i;
    uint64_t index = hash(value) % cache->size;
    
    for (i = 0; i < cache->capacity[index]; i++)
        if (cache->content[index * cache->n + i] == value)
            return 1; // cache hit
    
    return 0; // cache miss
}

void cache_insert (n_way_cache_t *cache, uint64_t value)
{
    uint64_t index = hash(value) % cache->size;
    size_t insert_pos = index * cache->n + (cache->capacity[index]++) % cache->n;

    cache->content[insert_pos] = value;
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
        fprintf(fp, "%d,%d,%d,%d,%d,%d\n", read_profile->p1_extend, read_profile->p2_extend, read_profile->p3_extend, read_profile->inv_psi, read_profile->num_seeds, read_profile->num_chains);
    } else {
       fp = fopen(filename, "w");
       fprintf(fp, "p1_extend,p2_extend,p3_extend,inv_psi,num_seeds,num_chains");
    }
    fclose(fp);
}
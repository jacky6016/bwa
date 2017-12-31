#include <stdint.h>
#include <stddef.h>

#include "profile.h"

#define BIT_MASK_32  ((1LL << 32) - 1)

void cache_lookup (n_way_cache_t *cache, uint64_t value)
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
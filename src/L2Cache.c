/**
 * @file    L2Cache.c
 * @author  Austin Glaser <austin@boulderes.com>
 * @brief   L2Cache Source
 *
 * @addtogroup L2CACHE
 * @{
 */

/* --- PRIVATE DEPENDENCIES ------------------------------------------------- */

#include "L2Cache.h"

#include "Access.h"
#include "MainMem.h"
#include "Util.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

/* --- PRIVATE CONSTANTS ---------------------------------------------------- */
/* --- PRIVATE DATATYPES ---------------------------------------------------- */

struct _l2_cache_t {
    main_mem_t        mem;
    cache_param_t   * configp;
    bool            has_been_accessed;
};

/* --- PRIVATE MACROS ------------------------------------------------------- */
/* --- PRIVATE FUNCTION PROTOTYPES ------------------------------------------ */
/* --- PUBLIC VARIABLES ----------------------------------------------------- */
/* --- PRIVATE VARIABLES ---------------------------------------------------- */
/* --- PUBLIC FUNCTIONS ----------------------------------------------------- */

l2_cache_t L2Cache_Create(main_mem_t mem, config_t * configp)
{
    l2_cache_t cache = (l2_cache_t) malloc(sizeof(*cache));
    if (cache == NULL) {
        return NULL;
    }

    cache->mem                     = mem;
    cache->configp                 = &configp->l2;
    cache->has_been_accessed       = false;

    return cache;
}

void L2Cache_Destroy(l2_cache_t cache)
{
    if (cache) free(cache);
}

uint32_t L2Cache_Access(l2_cache_t cache, access_t * accessp)
{
    uint32_t access_time_cycles = 0;

    if (!cache->has_been_accessed) {
        access_time_cycles += cache->configp->miss_time_cycles;

        access_t block_aligned_access;
        Access_Align(&block_aligned_access, accessp, cache->configp->block_size_bytes);

        access_time_cycles += MainMem_Access(cache->mem, &block_aligned_access);

        cache->has_been_accessed = true;
    }

    access_time_cycles += cache->configp->hit_time_cycles;

    return access_time_cycles;
}

/* --- PRIVATE FUNCTION DEFINITIONS ----------------------------------------- */

/** @} addtogroup L2CACHE */

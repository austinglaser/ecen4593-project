/**
 * @file    L1Cache.c
 * @author  Austin Glaser <austin@boulderes.com>
 * @brief   L1Cache Source
 *
 * @addtogroup L1CACHE
 * @{
 */

/* --- PRIVATE DEPENDENCIES ------------------------------------------------- */

#include "L1Cache.h"

#include "Access.h"
#include "CacheInternals.h"
#include "Config.h"
#include "L2Cache.h"
#include "Statistics.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

/* --- PRIVATE CONSTANTS ---------------------------------------------------- */
/* --- PRIVATE DATATYPES ---------------------------------------------------- */

struct _l1_cache_t {
    cache_t internals;
};

/* --- PRIVATE MACROS ------------------------------------------------------- */
/* --- PRIVATE FUNCTION PROTOTYPES ------------------------------------------ */

static uint32_t _L1Cache_AccessL2(void * _l2_cache, access_t const * access);

/* --- PUBLIC VARIABLES ----------------------------------------------------- */
/* --- PRIVATE VARIABLES ---------------------------------------------------- */
/* --- PUBLIC FUNCTIONS ----------------------------------------------------- */

l1_cache_t L1Cache_Create(l2_cache_t l2_cache, cache_stats_t * stats, cache_param_t const * config)
{
    l1_cache_t cache = (l1_cache_t) malloc(sizeof(*cache));
    if (cache == NULL) {
        return NULL;
    }

    cache->internals = CacheInternals_Create(_L1Cache_AccessL2, l2_cache, stats, config);
    if (cache->internals == NULL) {
        free(cache);
        return NULL;
    }

    return cache;
}

void L1Cache_Destroy(l1_cache_t cache)
{
    if (cache) {
        CacheInternals_Destroy(cache->internals);
        free(cache);
    }
}

uint32_t L1Cache_Access(l1_cache_t cache, access_t const * access)
{
    return CacheInternals_Access(cache->internals, access);
}

void L1Cache_Print(l1_cache_t l1_cache)
{
    CacheInternals_Print(l1_cache->internals);
}

/* --- PRIVATE FUNCTION DEFINITIONS ----------------------------------------- */

static uint32_t _L1Cache_AccessL2(void * _l2_cache, access_t const * access)
{
    l2_cache_t l2_cache = _l2_cache;
    return L2Cache_Access(l2_cache, access);
}

/** @} addtogroup L1CACHE */

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
#include "CacheInternals.h"
#include "Config.h"
#include "MainMem.h"
#include "Statistics.h"
#include "Util.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

/* --- PRIVATE CONSTANTS ---------------------------------------------------- */
/* --- PRIVATE DATATYPES ---------------------------------------------------- */

/**@brief   L2 does a little more work, because it needs to compute transfer
 *          time to L1 for the requested data
 */
struct _l2_cache_t {
    cache_t               internals;        /**< This does most of the work */
    cache_param_t const * config;           /**< The cache's config */
    uint32_t              bus_width_shift;  /**< A divide-shift to get the
                                                 busloads for a request */
};

/* --- PRIVATE MACROS ------------------------------------------------------- */
/* --- PRIVATE FUNCTION PROTOTYPES ------------------------------------------ */

/**@brief   Wrapper function to allow abstraction through CacheInternals */
static uint32_t _L2Cache_AccessMainMem(void * _main_mem, access_t const * access);

/* --- PUBLIC VARIABLES ----------------------------------------------------- */
/* --- PRIVATE VARIABLES ---------------------------------------------------- */
/* --- PUBLIC FUNCTIONS ----------------------------------------------------- */

l2_cache_t L2Cache_Create(main_mem_t mem, cache_stats_t * stats, cache_param_t const * config)
{
    l2_cache_t cache = (l2_cache_t) malloc(sizeof(*cache));
    if (cache == NULL) {
        return NULL;
    }

    cache->internals = CacheInternals_Create(_L2Cache_AccessMainMem, mem, stats, config);
    if (cache->internals == NULL) {
        free(cache);
        return NULL;
    }

    cache->config          = config;
    cache->bus_width_shift = HighestBitSet(config->bus_width_bytes);

    return cache;
}

void L2Cache_Destroy(l2_cache_t cache)
{
    if (cache) {
        CacheInternals_Destroy(cache->internals);
        free(cache);
    }
}

uint32_t L2Cache_Access(l2_cache_t cache, access_t const * access)
{
    uint32_t access_time_cycles = CacheInternals_Access(cache->internals, access);

    access_time_cycles += cache->config->transfer_time_cycles *
                          (access->n_bytes >> cache->bus_width_shift);


    return access_time_cycles;
}

void L2Cache_Print(l2_cache_t cache)
{
    CacheInternals_Print(cache->internals);
}

/* --- PRIVATE FUNCTION DEFINITIONS ----------------------------------------- */

static uint32_t _L2Cache_AccessMainMem(void * _main_mem, access_t const * access)
{
    main_mem_t main_mem = _main_mem;
    return MainMem_Access(main_mem, access);
}

/** @} addtogroup L2CACHE */

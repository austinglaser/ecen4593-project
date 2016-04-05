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

#include "CacheData.h"
#include "Config.h"
#include "L2Cache.h"
#include "Statistics.h"
#include "Util.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

/* --- PRIVATE CONSTANTS ---------------------------------------------------- */
/* --- PRIVATE DATATYPES ---------------------------------------------------- */

struct _l1_cache_t {
    l2_cache_t              l2_cache;
    cache_param_t const *   config;
    cache_stats_t *         stats;
    uint32_t                bus_width_shift;
    cache_data_t            data;
};

/* --- PRIVATE MACROS ------------------------------------------------------- */
/* --- PRIVATE FUNCTION PROTOTYPES ------------------------------------------ */
/* --- PUBLIC VARIABLES ----------------------------------------------------- */
/* --- PRIVATE VARIABLES ---------------------------------------------------- */
/* --- PUBLIC FUNCTIONS ----------------------------------------------------- */

l1_cache_t L1Cache_Create(l2_cache_t l2_cache, cache_stats_t * stats, cache_param_t const * config)
{
    l1_cache_t cache = (l1_cache_t) malloc(sizeof(*cache));
    if (cache == NULL) {
        return NULL;
    }

    uint32_t n_sets         = config->cache_size_bytes /
                              config->block_size_bytes /
                              config->associativity;
    uint32_t set_len        = config->associativity;
    cache->l2_cache         = l2_cache;
    cache->config           = config;
    cache->stats            = stats;
    cache->bus_width_shift  = HighestBitSet(config->bus_width_bytes);
    cache->data             = CacheData_Create(n_sets,
                                               set_len,
                                               config->block_size_bytes,
                                               8);

    return cache;
}

void L1Cache_Destroy(l1_cache_t cache)
{
    if (cache) {
        CacheData_Destroy(cache->data);
        free(cache);
    }
}

uint32_t L1Cache_Access(l1_cache_t cache, access_t const * access)
{
    result_t result;
    uint64_t dirty_kickout_address;
    if (access->type == TYPE_WRITE) {
        dirty_kickout_address = CacheData_Write(cache->data, access->address, &result);
    }
    else {
        dirty_kickout_address = CacheData_Read(cache->data, access->address, &result);
    }

    uint32_t access_time_cycles = 0;

    access_t dirty_write;
    access_t miss_read;
    switch (result) {
    case RESULT_MISS_DIRTY_KICKOUT:
        dirty_write.type    = TYPE_WRITE,
        dirty_write.address = dirty_kickout_address,
        dirty_write.n_bytes = cache->config->block_size_bytes,

        access_time_cycles += L2Cache_Access(cache->l2_cache, &dirty_write);
        // Intentional fallthrough

    case RESULT_MISS:
    case RESULT_MISS_KICKOUT:
        miss_read.type       = TYPE_READ,
        miss_read.address    = access->address,
        miss_read.n_bytes    = cache->config->block_size_bytes,

        access_time_cycles += cache->config->miss_time_cycles;
        access_time_cycles += L2Cache_Access(cache->l2_cache, &miss_read);
        break;

    default:
        break;
    }

    access_time_cycles += cache->config->hit_time_cycles;

    Statistics_RecordCacheAccess(cache->stats, result);

    return access_time_cycles;
}

/* --- PRIVATE FUNCTION DEFINITIONS ----------------------------------------- */

/** @} addtogroup L1CACHE */

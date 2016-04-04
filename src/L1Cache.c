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
#include "Util.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

/* --- PRIVATE CONSTANTS ---------------------------------------------------- */
/* --- PRIVATE DATATYPES ---------------------------------------------------- */

struct _l1_cache_t {
    l2_cache_t              l2_cache;
    cache_param_t const *   config;
    uint32_t                bus_width_shift;
    cache_data_t            data;
};

/* --- PRIVATE MACROS ------------------------------------------------------- */
/* --- PRIVATE FUNCTION PROTOTYPES ------------------------------------------ */
/* --- PUBLIC VARIABLES ----------------------------------------------------- */
/* --- PRIVATE VARIABLES ---------------------------------------------------- */
/* --- PUBLIC FUNCTIONS ----------------------------------------------------- */

l1_cache_t L1Cache_Create(l2_cache_t l2_cache, config_t const * config)
{
    l1_cache_t cache = (l1_cache_t) malloc(sizeof(*cache));
    if (cache == NULL) {
        return NULL;
    }

    uint32_t n_sets         = config->l1.cache_size_bytes /
                              config->l1.block_size_bytes /
                              config->l1.associativity;
    uint32_t set_len        = config->l1.associativity;
    cache->l2_cache         = l2_cache;
    cache->config           = &config->l1;
    cache->bus_width_shift  = HighestBitSet(config->l1.bus_width_bytes);
    cache->data             = CacheData_Create(n_sets,
                                               set_len,
                                               cache->config->block_size_bytes,
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
    access_t bus_aligned_access;
    Access_Align(&bus_aligned_access, access, 4);
    uint32_t n_bus_widths = bus_aligned_access.n_bytes / 4;

    uint32_t access_time_cycles = 0;
    uint32_t i;
    for (i = 0; i < n_bus_widths; i++) {
        result_t result;
        uint64_t bus_address = bus_aligned_access.address + i * 4;
        uint64_t dirty_kickout_address;
        if (access->type == TYPE_WRITE) {
            dirty_kickout_address = CacheData_Write(cache->data, bus_address, &result);
        }
        else {
            dirty_kickout_address = CacheData_Read(cache->data, bus_address, &result);
        }

        if (result == RESULT_MISS_DIRTY_KICKOUT) {
            access_t dirty_write = {
                .type    = TYPE_WRITE,
                .address = dirty_kickout_address,
                .n_bytes = cache->config->block_size_bytes,
            };

            access_time_cycles += L2Cache_Access(cache->l2_cache, &dirty_write);
        }

        if (result == RESULT_MISS || result == RESULT_MISS_DIRTY_KICKOUT) {
            access_t miss_read = {
                .type       = TYPE_READ,
                .address    = bus_address,
                .n_bytes    = cache->config->block_size_bytes,
            };

            access_time_cycles += cache->config->miss_time_cycles;
            access_time_cycles += L2Cache_Access(cache->l2_cache, &miss_read);
        }

        access_time_cycles += cache->config->hit_time_cycles;
    }

    return access_time_cycles;
}

/* --- PRIVATE FUNCTION DEFINITIONS ----------------------------------------- */

/** @} addtogroup L1CACHE */

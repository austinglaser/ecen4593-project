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
#include "CacheData.h"
#include "MainMem.h"
#include "Util.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

/* --- PRIVATE CONSTANTS ---------------------------------------------------- */
/* --- PRIVATE DATATYPES ---------------------------------------------------- */

struct _l2_cache_t {
    main_mem_t              mem;
    cache_param_t const *   config;
    uint32_t                bus_width_shift;
    cache_data_t            data;
};

/* --- PRIVATE MACROS ------------------------------------------------------- */
/* --- PRIVATE FUNCTION PROTOTYPES ------------------------------------------ */
/* --- PUBLIC VARIABLES ----------------------------------------------------- */
/* --- PRIVATE VARIABLES ---------------------------------------------------- */
/* --- PUBLIC FUNCTIONS ----------------------------------------------------- */

l2_cache_t L2Cache_Create(main_mem_t mem, config_t const * config)
{
    l2_cache_t cache = (l2_cache_t) malloc(sizeof(*cache));
    if (cache == NULL) {
        return NULL;
    }

    uint32_t n_sets         = config->l2.cache_size_bytes /
                              config->l2.block_size_bytes /
                              config->l2.associativity;
    uint32_t set_len        = config->l2.associativity;
    cache->mem              = mem;
    cache->config           = &config->l2;
    cache->bus_width_shift  = HighestBitSet(config->l2.bus_width_bytes);
    cache->data             = CacheData_Create(n_sets,
                                               set_len,
                                               cache->config->block_size_bytes,
                                               8);

    return cache;
}

void L2Cache_Destroy(l2_cache_t cache)
{
    if (cache) {
        CacheData_Destroy(cache->data);
        free(cache);
    }
}

uint32_t L2Cache_Access(l2_cache_t cache, access_t const * access)
{
    // We assume for the following code that the original access did not span
    // multiple L2 blocks. This effectively assumes that the L2 block size is a
    // multiple of the L1 block size

    result_t result;
    uint64_t dirty_kickout_address;
    if (access->type == TYPE_WRITE) {
        dirty_kickout_address = CacheData_Write(cache->data, access->address, &result);
    }
    else {
        dirty_kickout_address = CacheData_Read(cache->data, access->address, &result);
    }

    uint32_t access_time_cycles = 0;

    if (result == RESULT_MISS_DIRTY_KICKOUT) {
        access_t dirty_write = {
            .type    = TYPE_WRITE,
            .address = dirty_kickout_address,
            .n_bytes = cache->config->block_size_bytes,
        };

        access_time_cycles += MainMem_Access(cache->mem, &dirty_write);
    }

    if (result == RESULT_MISS || result == RESULT_MISS_DIRTY_KICKOUT) {
        access_t miss_read = {
            .type       = TYPE_READ,
            .address    = access->address,
            .n_bytes    = cache->config->block_size_bytes,
        };

        access_time_cycles += cache->config->miss_time_cycles;
        access_time_cycles += MainMem_Access(cache->mem, &miss_read);
    }

    access_time_cycles += cache->config->hit_time_cycles;
    access_time_cycles += cache->config->transfer_time_cycles *
                          (access->n_bytes >> cache->bus_width_shift);

    return access_time_cycles;
}

/* --- PRIVATE FUNCTION DEFINITIONS ----------------------------------------- */

/** @} addtogroup L2CACHE */

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
    main_mem_t      mem;
    cache_param_t   * configp;
    uint32_t        bus_width_shift;
    cache_data_t    data;
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

    uint32_t n_sets         = configp->l2.cache_size_bytes /
                              configp->l2.block_size_bytes /
                              configp->l2.associativity;
    uint32_t set_len        = configp->l2.associativity;
    cache->mem              = mem;
    cache->configp          = &configp->l2;
    cache->bus_width_shift  = HighestBitSet(configp->l2.bus_width_bytes);
    cache->data             = CacheData_Create(n_sets,
                                               set_len,
                                               cache->configp->block_size_bytes,
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

uint32_t L2Cache_Access(l2_cache_t cache, access_t * accessp)
{
    uint32_t access_time_cycles = 0;

    access_t block_aligned_access;
    Access_Align(&block_aligned_access, accessp, cache->configp->block_size_bytes);

    // We assume for the following code that the original access did not span
    // multiple L2 blocks. This effectively assumes that the L2 block size is a
    // multiple of the L1 block size

    result_t result;
    uint64_t dirty_kickout_address;
    if (block_aligned_access.type == TYPE_WRITE) {
        dirty_kickout_address = CacheData_Write(cache->data, block_aligned_access.address, &result);
    }
    else {
        dirty_kickout_address = CacheData_Read(cache->data, block_aligned_access.address, &result);
    }

    access_t dirty_write;
    switch (result) {
    case RESULT_MISS_DIRTY_KICKOUT:
        dirty_write.type    = TYPE_WRITE;
        dirty_write.address = dirty_kickout_address;
        dirty_write.n_bytes = cache->configp->block_size_bytes;

        access_time_cycles += MainMem_Access(cache->mem, &dirty_write);
        // Intentional fallthrough

    case RESULT_MISS:
        if (block_aligned_access.type == TYPE_WRITE) {
            block_aligned_access.type = TYPE_READ;
        }

        access_time_cycles += cache->configp->miss_time_cycles;
        access_time_cycles += MainMem_Access(cache->mem, &block_aligned_access);
        break;

    case RESULT_HIT:
    case RESULT_HIT_VICTIM_CACHE:
    default:
        break;
    }

    access_time_cycles += cache->configp->hit_time_cycles;
    access_time_cycles += cache->configp->transfer_time_cycles *
                          (accessp->n_bytes >> cache->bus_width_shift);

    return access_time_cycles;
}

/* --- PRIVATE FUNCTION DEFINITIONS ----------------------------------------- */

/** @} addtogroup L2CACHE */

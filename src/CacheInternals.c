/**
 * @file    CacheInternals.c
 * @author  Austin Glaser <austin@boulderes.com>
 * @brief   CacheInternals Source
 *
 * @addtogroup CACHEINTERNALS
 * @{
 */

/* --- PRIVATE DEPENDENCIES ------------------------------------------------- */

#include "CacheInternals.h"

#include "Access.h"
#include "Config.h"
#include "Statistics.h"
#include "Util.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

/* --- PRIVATE CONSTANTS ---------------------------------------------------- */
/* --- PRIVATE DATATYPES ---------------------------------------------------- */

struct _cache_t {
    cache_param_t const *   config;
    cache_stats_t *         stats;
    void *                  sub_mem;
    mem_access_f_t          sub_access_f;
    cache_data_t            data;
};

/* --- PRIVATE MACROS ------------------------------------------------------- */
/* --- PRIVATE FUNCTION PROTOTYPES ------------------------------------------ */
/* --- PUBLIC VARIABLES ----------------------------------------------------- */
/* --- PRIVATE VARIABLES ---------------------------------------------------- */
/* --- PUBLIC FUNCTIONS ----------------------------------------------------- */

cache_t CacheInternals_Create(mem_access_f_t sub_access_f, void * sub_mem, cache_stats_t * stats, cache_param_t const * config)
{
    cache_t cache = (cache_t) malloc(sizeof(*cache));
    if (cache == NULL) {
        return NULL;
    }

    uint32_t n_sets         = config->cache_size_bytes /
                              config->block_size_bytes /
                              config->associativity;
    uint32_t set_len        = config->associativity;
    cache->config           = config;
    cache->stats            = stats;
    cache->sub_mem          = sub_mem;
    cache->sub_access_f     = sub_access_f;
    cache->data             = CacheData_Create(n_sets,
                                               set_len,
                                               config->block_size_bytes,
                                               8);
    if (cache->data == NULL) {
        free(cache);
        return NULL;
    }

    return cache;
}

void CacheInternals_Destroy(cache_t cache)
{
    if (cache) {
        CacheData_Destroy(cache->data);
        free(cache);
    }
}

uint32_t CacheInternals_Access(cache_t cache, access_t const * access)
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

        access_time_cycles += cache->sub_access_f(cache->sub_mem, &dirty_write);
        // Intentional fallthrough

    case RESULT_MISS:
    case RESULT_MISS_KICKOUT:
        miss_read.type       = TYPE_READ,
        miss_read.address    = access->address,
        miss_read.n_bytes    = cache->config->block_size_bytes,

        access_time_cycles += cache->config->miss_time_cycles;
        access_time_cycles += cache->sub_access_f(cache->sub_mem, &miss_read);
        break;

    default:
        break;
    }

    access_time_cycles += cache->config->hit_time_cycles;

    Statistics_RecordCacheAccess(cache->stats, result);

    return access_time_cycles;
}

void CacheInternals_Print(cache_t cache)
{
    CacheData_Print(cache->data);
}

/* --- PRIVATE FUNCTION DEFINITIONS ----------------------------------------- */

/** @} addtogroup CACHEINTERNALS */

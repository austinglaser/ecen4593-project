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

/* --- PRIVATE CONSTANTS ---------------------------------------------------- */
/* --- PRIVATE DATATYPES ---------------------------------------------------- */
/* --- PRIVATE MACROS ------------------------------------------------------- */
/* --- PRIVATE FUNCTION PROTOTYPES ------------------------------------------ */
/* --- PUBLIC VARIABLES ----------------------------------------------------- */
/* --- PRIVATE VARIABLES ---------------------------------------------------- */
/* --- PUBLIC FUNCTIONS ----------------------------------------------------- */

void L2Cache_Create(l2_cache_t * cachep, main_mem_t mem, config_t * configp)
{
    cachep->mem                     = mem;
    cachep->configp                 = &configp->l2;
    cachep->has_been_accessed       = false;
}

uint32_t L2Cache_Access(l2_cache_t * cachep, access_t * accessp)
{
    uint32_t access_time_cycles = 0;

    if (!cachep->has_been_accessed) {
        access_time_cycles += cachep->configp->miss_time_cycles;

        access_t block_aligned_access;
        Access_Align(&block_aligned_access, accessp, cachep->configp->block_size_bytes);

        access_time_cycles += MainMem_Access(cachep->mem, &block_aligned_access);

        cachep->has_been_accessed = true;
    }

    access_time_cycles += cachep->configp->hit_time_cycles;

    return access_time_cycles;
}

/* --- PRIVATE FUNCTION DEFINITIONS ----------------------------------------- */

/** @} addtogroup L2CACHE */

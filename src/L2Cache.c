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

#include "Util.h"
#include "MainMem.h"

#include <stdbool.h>
#include <stdint.h>

/* --- PRIVATE CONSTANTS ---------------------------------------------------- */
/* --- PRIVATE DATATYPES ---------------------------------------------------- */
/* --- PRIVATE MACROS ------------------------------------------------------- */
/* --- PRIVATE FUNCTION PROTOTYPES ------------------------------------------ */
/* --- PUBLIC VARIABLES ----------------------------------------------------- */
/* --- PRIVATE VARIABLES ---------------------------------------------------- */
/* --- PUBLIC FUNCTIONS ----------------------------------------------------- */

void L2Cache_Create(l2_cache_t * cachep, main_mem_t * memp, config_t * configp)
{
    cachep->memp                    = memp;
    cachep->configp                 = &configp->l2;
    cachep->has_been_accessed       = false;
    cachep->block_alignment_mask    = BlockAlignmentMask(configp->l2.block_size_bytes);
}

uint32_t L2Cache_Access(l2_cache_t * cachep, access_t * accessp)
{
    uint32_t access_time_cycles = 0;

    if (!cachep->has_been_accessed) {
        access_time_cycles += cachep->configp->miss_time_cycles;

        uint64_t aligned_address = accessp->address & cachep->block_alignment_mask;
        uint64_t start_block = accessp->address & cachep->block_alignment_mask;
        uint64_t end_block = ( (accessp->address + accessp->n_bytes - 1) &
                               cachep->block_alignment_mask ) +
                             cachep->configp->block_size_bytes;
        uint64_t n_bytes = end_block - start_block;

        access_t main_mem_access = {
            .type = accessp->type,
            .address = aligned_address,
            .n_bytes = n_bytes,
        };
        access_time_cycles += MainMem_Access(cachep->memp, &main_mem_access);

        cachep->has_been_accessed = true;
    }

    access_time_cycles += cachep->configp->hit_time_cycles;

    return access_time_cycles;
}

/* --- PRIVATE FUNCTION DEFINITIONS ----------------------------------------- */

/** @} addtogroup L2CACHE */

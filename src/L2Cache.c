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
    cachep->memp    = memp;
    cachep->configp = &configp->l2;
}

uint32_t L2Cache_Access(l2_cache_t * cachep, access_t * accessp)
{
    return MainMem_Access(cachep->memp, accessp) + cachep->configp->miss_time_cycles;
}

/* --- PRIVATE FUNCTION DEFINITIONS ----------------------------------------- */

/** @} addtogroup L2CACHE */

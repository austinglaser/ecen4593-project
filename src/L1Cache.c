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
#include "L2Cache.h"

#include "Config.h"

#include <stdbool.h>
#include <stdint.h>

/* --- PRIVATE CONSTANTS ---------------------------------------------------- */
/* --- PRIVATE DATATYPES ---------------------------------------------------- */
/* --- PRIVATE MACROS ------------------------------------------------------- */
/* --- PRIVATE FUNCTION PROTOTYPES ------------------------------------------ */
/* --- PUBLIC VARIABLES ----------------------------------------------------- */
/* --- PRIVATE VARIABLES ---------------------------------------------------- */

static l2_cache_t * subcache;

/* --- PUBLIC FUNCTIONS ----------------------------------------------------- */

void L1Cache_Create(l1_cache_t * cachep, config_t * configp, l2_cache_t * l2_cache)
{
    (void) cachep;
    (void) configp;

    subcache = l2_cache;
}

uint32_t L1Cache_Access(l1_cache_t * cachep, access_t * accessp)
{
    (void) cachep;
    L2Cache_Access(subcache, accessp);

    return 35;
}

/* --- PRIVATE FUNCTION DEFINITIONS ----------------------------------------- */

/** @} addtogroup L1CACHE */

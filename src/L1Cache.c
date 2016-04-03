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
#include "Util.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

/* --- PRIVATE CONSTANTS ---------------------------------------------------- */
/* --- PRIVATE DATATYPES ---------------------------------------------------- */

struct _l1_cache_t {
};

/* --- PRIVATE MACROS ------------------------------------------------------- */
/* --- PRIVATE FUNCTION PROTOTYPES ------------------------------------------ */
/* --- PUBLIC VARIABLES ----------------------------------------------------- */
/* --- PRIVATE VARIABLES ---------------------------------------------------- */
/* --- PUBLIC FUNCTIONS ----------------------------------------------------- */

l1_cache_t L1Cache_Create(l2_cache_t l2_cache, config_t const * config)
{
    UNUSED_VARIABLE(l2_cache);
    UNUSED_VARIABLE(config);

    return NULL;
}

void L1Cache_Destroy(l1_cache_t cache)
{
    UNUSED_VARIABLE(cache);
}

uint32_t L1Cache_Access(l1_cache_t cache, access_t const * access)
{
    UNUSED_VARIABLE(cache);
    UNUSED_VARIABLE(access);
    return 0;
}

/* --- PRIVATE FUNCTION DEFINITIONS ----------------------------------------- */

/** @} addtogroup L1CACHE */

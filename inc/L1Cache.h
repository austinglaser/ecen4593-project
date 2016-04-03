/**
 * @file    L1Cache.h
 * @author  Austin Glaser <austin@boulderes.com>
 * @brief   L1Cache Interface
 */

#ifndef L1CACHE_H
#define L1CACHE_H

/**@defgroup L1CACHE L1Cache
 * @{ 
 *
 * @brief
 */

/* --- PUBLIC DEPENDENCIES -------------------------------------------------- */

#include "Config.h"
#include "Access.h"
#include "L2Cache.h"

#include <stdbool.h>
#include <stdint.h>

/* --- PUBLIC CONSTANTS ----------------------------------------------------- */
/* --- PUBLIC DATATYPES ----------------------------------------------------- */

typedef struct _l1_cache_t * l1_cache_t;

/* --- PUBLIC MACROS -------------------------------------------------------- */
/* --- PUBLIC VARIABLES ----------------------------------------------------- */
/* --- PUBLIC FUNCTIONS ----------------------------------------------------- */

l1_cache_t L1Cache_Create(l2_cache_t l2_cache, config_t const * config);
void L1Cache_Destroy(l1_cache_t cache);
uint32_t L1Cache_Access(l1_cache_t cache, access_t const * access);

/** @} defgroup L1CACHE */

#endif /* ifndef L1CACHE_H */

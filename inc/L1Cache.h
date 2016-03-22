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

typedef struct {
    bool has_been_accessed;
} l1_cache_t;

/* --- PUBLIC MACROS -------------------------------------------------------- */
/* --- PUBLIC VARIABLES ----------------------------------------------------- */
/* --- PUBLIC FUNCTIONS ----------------------------------------------------- */

void L1Cache_Create(l1_cache_t * cachep, config_t * configp, l2_cache_t * l2_cachep);
uint32_t L1Cache_Access(l1_cache_t * cachep, access_t * accessp);

/** @} defgroup L1CACHE */

#endif /* ifndef L1CACHE_H */

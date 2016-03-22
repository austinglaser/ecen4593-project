/**
 * @file    L2Cache.h
 * @author  Austin Glaser <austin@boulderes.com>
 * @brief   L2Cache Interface
 */

#ifndef L2CACHE_H
#define L2CACHE_H

/**@defgroup L2CACHE L2Cache
 * @{ 
 *
 * @brief
 */

/* --- PUBLIC DEPENDENCIES -------------------------------------------------- */

#include "Config.h"
#include "Access.h"

#include <stdbool.h>
#include <stdint.h>

/* --- PUBLIC CONSTANTS ----------------------------------------------------- */
/* --- PUBLIC DATATYPES ----------------------------------------------------- */

typedef struct {
    uint32_t dummy;
} l2_cache_t;

/* --- PUBLIC MACROS -------------------------------------------------------- */
/* --- PUBLIC VARIABLES ----------------------------------------------------- */
/* --- PUBLIC FUNCTIONS ----------------------------------------------------- */

void L2Cache_Create(l2_cache_t * cachep, config_t * configp);
uint32_t L2Cache_Access(l2_cache_t * cachep, access_t * accessp);

/** @} defgroup L2CACHE */

#endif /* ifndef L2CACHE_H */

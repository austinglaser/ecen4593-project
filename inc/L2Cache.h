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

#include "MainMem.h"

#include <stdbool.h>
#include <stdint.h>

/* --- PUBLIC CONSTANTS ----------------------------------------------------- */
/* --- PUBLIC DATATYPES ----------------------------------------------------- */

typedef struct _l2_cache_t * l2_cache_t;

/* --- PUBLIC MACROS -------------------------------------------------------- */
/* --- PUBLIC VARIABLES ----------------------------------------------------- */
/* --- PUBLIC FUNCTIONS ----------------------------------------------------- */

l2_cache_t L2Cache_Create(main_mem_t mem, cache_param_t const * config);
void L2Cache_Destroy(l2_cache_t cache);
uint32_t L2Cache_Access(l2_cache_t cache, access_t const * access);

/** @} defgroup L2CACHE */

#endif /* ifndef L2CACHE_H */

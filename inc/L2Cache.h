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
 * @brief L2 Cache level. A thin wrapper around @ref CACHEINTERNALS
 */

/* --- PUBLIC DEPENDENCIES -------------------------------------------------- */

#include "Access.h"
#include "Config.h"
#include "MainMem.h"
#include "Statistics.h"

#include <stdbool.h>
#include <stdint.h>

/* --- PUBLIC CONSTANTS ----------------------------------------------------- */
/* --- PUBLIC DATATYPES ----------------------------------------------------- */

/**@breif   Instance of an L2 cache */
typedef struct _l2_cache_t * l2_cache_t;

/* --- PUBLIC MACROS -------------------------------------------------------- */
/* --- PUBLIC VARIABLES ----------------------------------------------------- */
/* --- PUBLIC FUNCTIONS ----------------------------------------------------- */

/**@brief   Create a new L2 cache instance
 *
 * @param[in] main_mem:     The external memory to access when a miss or
 *                          dirty-kickout occurs
 * @param[in] stats:        Where statistics about cache performance will be
 *                          written
 * @param[in] config:       Descriptor containing the cache's parameters
 *
 * @return      A new L2 cache instance, or NULL if memory allocation failed
 */
l2_cache_t L2Cache_Create(main_mem_t mem,
                          cache_stats_t * stats,
                          cache_param_t const * config);

/**@brief   Destroy an L2 cache instance
 *
 * @param[in] cache:        The cache to be destroyed
 */
void L2Cache_Destroy(l2_cache_t cache);

/**@brief   Simulate an access to an L2 cache
 *
 * @param[in,out] cache:    The cache to access
 * @param[in] access:       Access descriptor
 *
 * @return  The total number of cycles to resolve the access
 */
uint32_t L2Cache_Access(l2_cache_t cache, access_t const * access);

/**@brief   Print the current cache contents
 *
 * @param[in] cache:        The cache instance to print
 */
void L2Cache_Print(l2_cache_t cache);

/** @} defgroup L2CACHE */

#endif /* ifndef L2CACHE_H */

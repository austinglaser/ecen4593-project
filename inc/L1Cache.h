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
 * @brief L1 Cache level (both instruction and data). A thin wrapper around @ref CACHEINTERNALS
 */

/* --- PUBLIC DEPENDENCIES -------------------------------------------------- */

#include "Access.h"
#include "Config.h"
#include "L2Cache.h"
#include "Statistics.h"

#include <stdbool.h>
#include <stdint.h>

/* --- PUBLIC CONSTANTS ----------------------------------------------------- */
/* --- PUBLIC DATATYPES ----------------------------------------------------- */

/**@brief   Instance of an L1 cache */
typedef struct _l1_cache_t * l1_cache_t;

/* --- PUBLIC MACROS -------------------------------------------------------- */
/* --- PUBLIC VARIABLES ----------------------------------------------------- */
/* --- PUBLIC FUNCTIONS ----------------------------------------------------- */

/**@brief   Create a new l1 cache instance
 *
 * @param[in] l2_cache:     The l2_cache to access when a miss or dirty-kickout
 *                          occurs
 * @param[in] stats:        Where statistics about cache performance will be written
 * @param[in] config:       Descriptor containing the cache's parameters
 *
 * @return      A new l1 cache instance, or NULL if memory allocation failed
 */
l1_cache_t L1Cache_Create(l2_cache_t l2_cache, cache_stats_t * stats, cache_param_t const * config);

/**@brief   Destroy an L1 cache instance
 *
 * @param[in] cache:        The cache to be destroyed
 */
void L1Cache_Destroy(l1_cache_t cache);

/**@brief   Simulate an access to an L1 cache
 *
 * @param[in,out] cache:    The cache to access
 * @param[in] access:       Access descriptor
 *
 * @return  The total number of cycles to resolve the access
 */
uint32_t L1Cache_Access(l1_cache_t cache, access_t const * access);

/**@brief   Print the current cache contents
 *
 * @param[in] l1_cache:     The cache instance to print
 */
void L1Cache_Print(l1_cache_t l1_cache);

/** @} defgroup L1CACHE */

#endif /* ifndef L1CACHE_H */

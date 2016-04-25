/**
 * @file    CacheInternals.h
 * @author  Austin Glaser <austin@boulderes.com>
 * @brief   CacheInternals Interface
 */

#ifndef CACHEINTERNALS_H
#define CACHEINTERNALS_H

/**@defgroup CACHEINTERNALS CacheInternals
 * @{
 *
 * @brief   Simulates a single memory level, handling all interaction (incoming
 *          and outgoing accesses) at this level, and to the next.
 */

/* --- PUBLIC DEPENDENCIES -------------------------------------------------- */

#include "Access.h"
#include "Config.h"
#include "Statistics.h"

#include <stdbool.h>
#include <stdint.h>

/* --- PUBLIC CONSTANTS ----------------------------------------------------- */
/* --- PUBLIC DATATYPES ----------------------------------------------------- */

/**@brief   Type of a cache */
typedef struct _cache_t * cache_t;

/**@brief   Abstraction of a memory access operation
 *
 * @param[in,out] mem:      The memory level to access
 * @param[in] access:       Access descriptor
 *
 * @return                  The number of cycles it took to perform @p access
 */
typedef uint32_t (*mem_access_f_t)(void * mem, access_t const * access);

/* --- PUBLIC MACROS -------------------------------------------------------- */
/* --- PUBLIC VARIABLES ----------------------------------------------------- */
/* --- PUBLIC FUNCTIONS ----------------------------------------------------- */

/**@brief   Create a new CacheInternals instance
 *
 * @param[in] sub_access_f:     A function that can be used to access the
 *                              next-lower level of memory, when needed
 * @param[in] sub_mem:          A pointer to that next memory level
 * @param[out] stats:           The structure where bookkeeping data will be
 *                              written
 * @param[in] config:           Structure controlling the cache's parameters
 *
 * @return                      A new CacheInternals instance, or NULL if
 *                              memory allocation failed
 */
cache_t CacheInternals_Create(mem_access_f_t sub_access_f,
                              void * sub_mem,
                              cache_stats_t * stats,
                              cache_param_t const * config);

/**@brief   Destroys a CacheInternals instance, freeing all memory used
 *
 * @param[in] cache:            The cache to free
 */
void CacheInternals_Destroy(cache_t cache);

/**@brief   Simulate an access to this memory level
 *
 * Accesses the next memory level if a miss (and possibly dirty kickout)
 * occurred
 *
 * @param[in,out] cache:        The cache to access
 * @param[in] access:           Access descriptor
 *
 * @return                      The total cycles the access consumed
 */
uint32_t CacheInternals_Access(cache_t cache, access_t const * access);

/**@brief   Prints the cache's current state
 *
 * @note    Thin wrapper around @ref CacheData_Print()
 */
void CacheInternals_Print(cache_t cache);

/** @} defgroup CACHEINTERNALS */

#endif /* ifndef CACHEINTERNALS_H */

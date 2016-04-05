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
 * @brief
 */

/* --- PUBLIC DEPENDENCIES -------------------------------------------------- */

#include "Access.h"
#include "Config.h"
#include "Statistics.h"

#include <stdbool.h>
#include <stdint.h>

/* --- PUBLIC CONSTANTS ----------------------------------------------------- */
/* --- PUBLIC DATATYPES ----------------------------------------------------- */

typedef struct _cache_t * cache_t;

typedef uint32_t (*mem_access_f_t)(void * mem, access_t const * access);

/* --- PUBLIC MACROS -------------------------------------------------------- */
/* --- PUBLIC VARIABLES ----------------------------------------------------- */
/* --- PUBLIC FUNCTIONS ----------------------------------------------------- */

cache_t CacheInternals_Create(mem_access_f_t sub_access_f, void * sub_mem, cache_stats_t * stats, cache_param_t const * config);
void CacheInternals_Destroy(cache_t cache);
uint32_t CacheInternals_Access(cache_t cache, access_t const * access);

/** @} defgroup CACHEINTERNALS */

#endif /* ifndef CACHEINTERNALS_H */

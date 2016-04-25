/**
 * @file    Statistics.h
 * @author  Austin Glaser <austin@boulderes.com>
 * @brief   Statistics Interface
 */

#ifndef STATISTICS_H
#define STATISTICS_H

/**@defgroup STATISTICS Statistics
 * @{
 *
 * @brief   Provides a central location to record all simulation statistics
 */

/* --- PUBLIC DEPENDENCIES -------------------------------------------------- */

#include "Access.h"
#include "CacheData.h"

#include <stdbool.h>
#include <stdint.h>

/* --- PUBLIC CONSTANTS ----------------------------------------------------- */
/* --- PUBLIC DATATYPES ----------------------------------------------------- */

/**@brief   Statistics for a single cache */
typedef struct _cache_stats_t {
    const char * name;              /**< The cache's name (used for printing) */
    uint64_t hit_count;             /**< The number of accesses which resulted
                                         in a hit */
    uint64_t miss_count;            /**< The number of accesses which resulted
                                         in a miss */
    uint64_t kickouts;              /**< The number of blocks kicked out by new
                                         data */
    uint64_t dirty_kickouts;        /**< The number of *dirty* blocks kicked
                                         out by new data (resulting in a write
                                         access to the next memory level) */
    uint64_t transfers;             /**< The number of read accesses to the
                                         next memory level */
    uint64_t vc_hit_count;          /**< The number of times a requested block
                                         was found in this cache's victim cache */
} cache_stats_t;

/**@brief   Top-level statistics structure */
typedef struct _stats_t {
    uint64_t read_count;            /**< Total number of read accesses */
    uint64_t read_count_aligned;    /**< Total number of L1 read accesses */
    uint64_t read_cycles;           /**< Total cycles spent reading memory */

    uint64_t write_count;           /**< Total number of write accesses */
    uint64_t write_count_aligned;   /**< Total number of L1 write accesses */
    uint64_t write_cycles;          /**< Total cycles spent writing memory */

    uint64_t instr_count;           /**< Total number of instruction reads */
    uint64_t instr_count_aligned;   /**< Total number of L1 instruction reads */
    uint64_t instr_cycles;          /**< Total cycles spent retrieving
                                         instructions */

    cache_stats_t l1i;              /**< Individual statistics for L1
                                         instruction cache */
    cache_stats_t l1d;              /**< Individual statistics for L1 data
                                         cache */
    cache_stats_t l2;               /**< Individual statistics for L2 cache */
} stats_t;

/* --- PUBLIC MACROS -------------------------------------------------------- */
/* --- PUBLIC VARIABLES ----------------------------------------------------- */
/* --- PUBLIC FUNCTIONS ----------------------------------------------------- */

/**@brief   Initialize a statistics structure
 *
 * @param[out] stats:       The structure to be populated
 */
void Statistics_Create(stats_t * stats);

/**@brief   Print accumulated statistics
 *
 * @param[in] stats:        The data to print
 */
void Statistics_Print(stats_t const * stats);

/**@brief   Record a top-level memory access (line in the trace file)
 *
 * @param[in,out] stats:    Location to store data
 * @param[in] type:         The type of the access
 * @param[in] cycles:       Total cycles to resolve the access
 * @param[in] n_aligned:    The actual number of L1 accesses this request
 *                          caused
 */
void Statistics_RecordAccess(stats_t * stats,
                             uint8_t type,
                             uint32_t cycles,
                             uint32_t n_aligned);

/**@brief   Record a single access to a cache
 *
 * @param[in,out] cache_stats:  This cache's statistics
 * @param[in] result:           The access result (miss, hit, etc)
 */
void Statistics_RecordCacheAccess(cache_stats_t * cache_stats, result_t result);

/** @} defgroup STATISTICS */

#endif /* ifndef STATISTICS_H */

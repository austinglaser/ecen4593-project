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
 * @brief
 */

/* --- PUBLIC DEPENDENCIES -------------------------------------------------- */

#include "Access.h"

#include <stdbool.h>
#include <stdint.h>

/* --- PUBLIC CONSTANTS ----------------------------------------------------- */
/* --- PUBLIC DATATYPES ----------------------------------------------------- */

typedef struct _cache_stats_t {
    const char * name;
    uint64_t hit_count;
    uint64_t miss_count;
    uint64_t kickouts;
    uint64_t dirty_kickouts;
    uint64_t transfers;
    uint64_t vc_hit_count;
} cache_stats_t;

typedef struct _stats_t {
    uint64_t read_count;
    uint64_t read_count_aligned;
    uint64_t read_cycles;

    uint64_t write_count;
    uint64_t write_count_aligned;
    uint64_t write_cycles;

    uint64_t instr_count;
    uint64_t instr_count_aligned;
    uint64_t instr_cycles;

    cache_stats_t l1i;
    cache_stats_t l1d;
    cache_stats_t l2;
} stats_t;

/* --- PUBLIC MACROS -------------------------------------------------------- */
/* --- PUBLIC VARIABLES ----------------------------------------------------- */
/* --- PUBLIC FUNCTIONS ----------------------------------------------------- */

void Statistics_Create(stats_t * stats);

void Statistics_RecordAccess(stats_t * stats, access_t const * access, uint32_t cycles, uint32_t n_aligned);

/** @} defgroup STATISTICS */

#endif /* ifndef STATISTICS_H */

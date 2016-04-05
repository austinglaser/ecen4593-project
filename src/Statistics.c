/**
 * @file    Statistics.c
 * @author  Austin Glaser <austin@boulderes.com>
 * @brief   Statistics Source
 *
 * @addtogroup STATISTICS
 * @{
 */

/* --- PRIVATE DEPENDENCIES ------------------------------------------------- */

#include "Statistics.h"

#include "Util.h"

#include <stdbool.h>
#include <stdint.h>
#include <string.h>

/* --- PRIVATE CONSTANTS ---------------------------------------------------- */
/* --- PRIVATE DATATYPES ---------------------------------------------------- */
/* --- PRIVATE MACROS ------------------------------------------------------- */
/* --- PRIVATE FUNCTION PROTOTYPES ------------------------------------------ */
/* --- PUBLIC VARIABLES ----------------------------------------------------- */
/* --- PRIVATE VARIABLES ---------------------------------------------------- */
/* --- PUBLIC FUNCTIONS ----------------------------------------------------- */

void Statistics_Create(stats_t * stats)
{
    memset(stats, 0, sizeof(*stats));

    stats->l1i.name = "L1i";
    stats->l1d.name = "L1d";
    stats->l2.name = "L2";
}

void Statistics_RecordAccess(stats_t * stats, uint8_t type, uint32_t cycles, uint32_t n_aligned)
{
    switch(type) {
    case TYPE_READ:
        stats->read_count         += 1;
        stats->read_count_aligned += n_aligned;
        stats->read_cycles        += cycles;
        break;
    case TYPE_WRITE:
        stats->write_count         += 1;
        stats->write_count_aligned += n_aligned;
        stats->write_cycles        += cycles;
        break;
    case TYPE_INSTR:
        stats->instr_count         += 1;
        stats->instr_count_aligned += n_aligned;
        stats->instr_cycles        += cycles;
        break;
    }
}

void Statistics_RecordCacheAccess(cache_stats_t * cache_stats, result_t result)
{
    switch (result) {
    case RESULT_HIT_VICTIM_CACHE:
        cache_stats->vc_hit_count += 1;
        // Intentional fallthrough

    case RESULT_HIT:
        cache_stats->hit_count += 1;
        break;

    case RESULT_MISS_DIRTY_KICKOUT:
        cache_stats->dirty_kickouts += 1;
        // Intentional fallthrough

    case RESULT_MISS_KICKOUT:
        cache_stats->kickouts += 1;
        // Intentional fallthrough

    case RESULT_MISS:
        cache_stats->miss_count += 1;
        cache_stats->transfers += 1;
        break;
    }

}

/* --- PRIVATE FUNCTION DEFINITIONS ----------------------------------------- */

/** @} addtogroup STATISTICS */

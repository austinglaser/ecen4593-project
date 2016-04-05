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

#include <inttypes.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>

/* --- PRIVATE CONSTANTS ---------------------------------------------------- */
/* --- PRIVATE DATATYPES ---------------------------------------------------- */
/* --- PRIVATE MACROS ------------------------------------------------------- */
/* --- PRIVATE FUNCTION PROTOTYPES ------------------------------------------ */

static double Statistics_Percentage(uint64_t number, uint64_t total);

static double Statistics_CPI(uint64_t cycles, uint64_t instructions);

static void Statistics_PrintCache(cache_stats_t const * cache_stats);

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

void Statistics_Print(stats_t const * stats)
{
    uint64_t total_cycles = stats->read_cycles + stats->write_cycles + stats->instr_cycles;
    uint64_t total_refs = stats->read_count  + stats->write_count  + stats->instr_count;
    printf("Execute time = %" PRIu64 "; Total refs = %" PRIu64 "\n",
           total_cycles,
           total_refs);
    printf("Inst refs = %" PRIu64 "; Data refs = %" PRIu64 "\n",
           stats->instr_count,
           stats->read_count + stats->write_count);
    printf("\n");

    printf("Number of reference types:   [Percentage]\n");
    printf("  Reads  = %12" PRIu64 "      [%4.1f%%]\n",
           stats->read_count,
           Statistics_Percentage(stats->read_count, total_refs));
    printf("  Writes = %12" PRIu64 "      [%4.1f%%]\n",
           stats->write_count,
           Statistics_Percentage(stats->write_count, total_refs));
    printf("  Inst.  = %12" PRIu64 "      [%4.1f%%]\n",
           stats->instr_count,
           Statistics_Percentage(stats->instr_count, total_refs));
    printf("  Total  = %12" PRIu64 "\n", total_refs);
    printf("\n");

    printf("Total cycles for activities: [Percentage]\n");
    printf("  Reads  = %12" PRIu64 "      [%4.1f%%]\n",
           stats->read_cycles,
           Statistics_Percentage(stats->read_cycles, total_cycles));
    printf("  Writes = %12" PRIu64 "      [%4.1f%%]\n",
           stats->write_cycles,
           Statistics_Percentage(stats->write_cycles, total_cycles));
    printf("  Inst.  = %12" PRIu64 "      [%4.1f%%]\n",
           stats->instr_cycles,
           Statistics_Percentage(stats->instr_cycles, total_cycles));
    printf("  Total  = %12" PRIu64 "\n", total_cycles);
    printf("\n");

    printf("CPI = %5.1f\n", Statistics_CPI(total_cycles, stats->instr_count));
    uint64_t total_ideal_cycles = total_refs + stats->instr_count;
    printf("Ideal:             Exec. Time = %12" PRIu64 "; CPI = %5.1f\n",
           total_ideal_cycles,
           Statistics_CPI(total_ideal_cycles, stats->instr_count));
    uint64_t total_aligned_cycles = stats->read_count_aligned +
                                    stats->write_count_aligned +
                                    stats->instr_count_aligned +
                                    stats->instr_count;
    printf("Ideal mis-aligned: Exec. Time = %12" PRIu64 "; CPI = %5.1f\n",
           total_aligned_cycles,
           Statistics_CPI(total_aligned_cycles, stats->instr_count));
    printf("\n");

    Statistics_PrintCache(&(stats->l1i));
    printf("\n");

    Statistics_PrintCache(&(stats->l1d));
    printf("\n");

    Statistics_PrintCache(&(stats->l2));
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

static double Statistics_Percentage(uint64_t number, uint64_t total)
{
    double dnumber = (double) number;
    double dtotal = (double) total;

    return (dnumber / dtotal) * 100.0;
}

static double Statistics_CPI(uint64_t cycles, uint64_t instructions)
{
    double dcycles = (double) cycles;
    double dinstrs = (double) instructions;

    return dcycles / dinstrs;
}

static void Statistics_PrintCache(cache_stats_t const * cache_stats)
{
    uint64_t total_requests = cache_stats->hit_count + cache_stats->miss_count;
    printf("Memory level: %s\n", cache_stats->name);
    printf("  Hit Count = %" PRIu64 "  Miss Count = %" PRIu64 "\n",
           cache_stats->hit_count,
           cache_stats->miss_count);
    printf("  Total Requests = %" PRIu64 "\n",
           total_requests);
    printf("  Hit rate = %4.1f%%  Miss Rate = %4.1f%%\n",
           Statistics_Percentage(cache_stats->hit_count,  total_requests),
           Statistics_Percentage(cache_stats->miss_count, total_requests));
    printf("  Kickouts = %" PRIu64 "; Dirty kickouts = %" PRIu64 "; Transfers = %" PRIu64 "\n",
           cache_stats->kickouts,
           cache_stats->dirty_kickouts,
           cache_stats->transfers);
    printf("  VC Hit count = %" PRIu64 "\n",
           cache_stats->vc_hit_count);
}

/** @} addtogroup STATISTICS */

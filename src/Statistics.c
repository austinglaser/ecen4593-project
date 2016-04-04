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

void Statistics_RecordAccess(stats_t * stats, access_t const * access, uint32_t cycles)
{
    if (access->type == TYPE_READ) {
        stats->read_count += 1;
        stats->read_cycles += cycles;
    }
    else if (access->type == TYPE_WRITE) {
        stats->write_count += 1;
        stats->write_cycles += cycles;
    }
    else if (access->type == TYPE_INSTR) {
        stats->instr_count += 1;
        stats->instr_cycles += cycles;
    }
}

/* --- PRIVATE FUNCTION DEFINITIONS ----------------------------------------- */

/** @} addtogroup STATISTICS */

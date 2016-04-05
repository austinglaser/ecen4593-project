/**
 * @file    main.c
 * @author  Austin Glaser <austin@boulderes.com>
 * @brief   Application Entry Point
 *
 * @defgroup MAIN Main
 * @{
 */

/* --- PRIVATE DEPENDENCIES ------------------------------------------------- */

#include "Config.h"
#include "Statistics.h"

#include <inttypes.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

/* --- PRIVATE DATATYPES ---------------------------------------------------- */
/* --- PRIVATE FUNCTION PROTOTYPES ------------------------------------------ */
/* --- PRIVATE CONSTANTS ---------------------------------------------------- */

#undef FAKE_STATS

/* --- PRIVATE MACROS ------------------------------------------------------- */
/* --- PUBLIC VARIABLES ----------------------------------------------------- */
/* --- PRIVATE VARIABLES ---------------------------------------------------- */
/* --- PUBLIC FUNCTIONS ----------------------------------------------------- */

/**
 * @brief   Application Entry Point
 */
int main(int argc, char const * const * const argv)
{
    if (argc > 2) {
        printf("Usage: %s [config_file]\n", argv[0]);
        return -1;
    }

    char const * config_file = NULL;
    if (argc == 2) {
        config_file = argv[1];
    }

    config_t config;
    Config_FromFile(config_file, &config);

    printf("\nMemory system:\n");
    Config_Print(&config);
    printf("\n");

    stats_t stats;
    Statistics_Create(&stats);

#ifdef FAKE_STATS
    stats.read_count          = 940933865;
    stats.read_count_aligned  = stats.read_count * 2;
    stats.read_cycles         = 6362234282;

    stats.write_count         = 374031357;
    stats.write_count_aligned = stats.write_count * 2;
    stats.write_cycles        = 5668329310;

    stats.instr_count         = 3685034778;
    stats.instr_count_aligned = stats.instr_count * 2;
    stats.instr_cycles        = 18577582319;

    stats.l1i.hit_count       = 6162531865;
    stats.l1i.miss_count      = 108939899;
    stats.l1i.kickouts        = 103082777;
    stats.l1i.dirty_kickouts  = 0;
    stats.l1i.transfers       = 103083041;
    stats.l1i.vc_hit_count    = 5856858;

    stats.l1d.hit_count       = 1622984580;
    stats.l1d.miss_count      = 66008903;
    stats.l1d.kickouts        = 57344834;
    stats.l1d.dirty_kickouts  = 30336644;
    stats.l1d.transfers       = 57345098;
    stats.l1d.vc_hit_count    = 8663805;

    stats.l2.hit_count        = 132055819;
    stats.l2.miss_count       = 58708964;
    stats.l2.kickouts         = 56097023;
    stats.l2.dirty_kickouts   = 15342421;
    stats.l2.transfers        = 56097543;
    stats.l2.vc_hit_count     = 2611421;
#else
#endif

    Statistics_Print(&stats);
    printf("\n");

    return 0;
}

/* --- PRIVATE FUNCTION DEFINITIONS ----------------------------------------- */

/** @} defgroup MAIN */

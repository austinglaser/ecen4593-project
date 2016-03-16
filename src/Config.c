/**
 * @file    Config.c
 * @author  Austin Glaser <austin@boulderes.com>
 * @brief   Config Source
 *
 * @addtogroup CONFIG
 * @{
 */

/* --- PRIVATE DEPENDENCIES ------------------------------------------------- */

#include "Config.h"

#include <stdbool.h>
#include <stdint.h>

/* --- PRIVATE CONSTANTS ---------------------------------------------------- */
/* --- PRIVATE DATATYPES ---------------------------------------------------- */
/* --- PRIVATE MACROS ------------------------------------------------------- */
/* --- PRIVATE FUNCTION PROTOTYPES ------------------------------------------ */
/* --- PUBLIC VARIABLES ----------------------------------------------------- */
/* --- PRIVATE VARIABLES ---------------------------------------------------- */
/* --- PUBLIC FUNCTIONS ----------------------------------------------------- */
/* --- PRIVATE FUNCTION DEFINITIONS ----------------------------------------- */

void Config_Defaults(config_t * configp)
{
    configp->l1.block_size_bytes        = 32;
    configp->l1.cache_size_bytes        = 8192;
    configp->l1.is_associative          = true;
    configp->l1.hit_time_cycles         = 1;
    configp->l1.miss_time_cycles        = 1;
    configp->l1.transfer_time_cycles    = 0;    // not valid for L1
    configp->l1.bus_width_bytes         = 0;    // not valid for L1

    configp->l2.block_size_bytes        = 64;
    configp->l2.cache_size_bytes        = 32768;
    configp->l2.is_associative          = true;
    configp->l2.hit_time_cycles         = 8;
    configp->l2.miss_time_cycles        = 10;
    configp->l2.transfer_time_cycles    = 10;
    configp->l2.bus_width_bytes         = 16;
}

void Config_ParseLine(const char * line, config_t * configp)
{
    (void) line;
    (void) configp;
}

/** @} addtogroup CONFIG */

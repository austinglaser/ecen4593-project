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

#include <inttypes.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>

/* --- PRIVATE CONSTANTS ---------------------------------------------------- */

static const char * l1_str          = "L1";
static const char * block_size_str  = "block_size";

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
    configp->l1.associative             = 1;
    configp->l1.hit_time_cycles         = 1;
    configp->l1.miss_time_cycles        = 1;
    configp->l1.transfer_time_cycles    = 0;    // not valid for L1
    configp->l1.bus_width_bytes         = 0;    // not valid for L1

    configp->l2.block_size_bytes        = 64;
    configp->l2.cache_size_bytes        = 32768;
    configp->l2.associative             = 1;
    configp->l2.hit_time_cycles         = 8;
    configp->l2.miss_time_cycles        = 10;
    configp->l2.transfer_time_cycles    = 10;
    configp->l2.bus_width_bytes         = 16;
}

void Config_ParseLine(const char * line, config_t * configp)
{
    char cache_str[64];
    char field_str[64];
    uint32_t value;

    if (sscanf(line, "%[^_]_%[^=]=%" SCNu32 "\n", cache_str, field_str, &value) == 3) {
        if (strcmp(cache_str, l1_str) == 0) {
            if (strcmp(field_str, block_size_str) == 0) {
                configp->l1.block_size_bytes = value;
            }
        }
    }
}

/** @} addtogroup CONFIG */

/**
 * @file    ConfigDefaults.c
 * @author  Austin Glaser <austin@boulderes.com>
 * @brief   ConfigDefaults Source
 *
 * @addtogroup CONFIGDEFAULTS
 * @{
 */

/* --- PRIVATE DEPENDENCIES ------------------------------------------------- */

#include "ConfigDefaults.h"

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

void SetDefaultConfigValues(config_t * configp)
{
    configp->l1.block_size_bytes            = 32;
    configp->l1.cache_size_bytes            = 8192;
    configp->l1.associativity               = 1;
    configp->l1.hit_time_cycles             = 1;
    configp->l1.miss_time_cycles            = 1;
    configp->l1.transfer_time_cycles        = 0;  // Invalid
    configp->l1.bus_width_bytes             = 0;  // Invalid

    configp->l2.block_size_bytes            = 64;
    configp->l2.cache_size_bytes            = 32768;
    configp->l2.associativity               = 1;
    configp->l2.hit_time_cycles             = 8;
    configp->l2.miss_time_cycles            = 10;
    configp->l2.transfer_time_cycles        = 10;
    configp->l2.bus_width_bytes             = 16;

    configp->main_mem.send_address_cycles   = 10;
    configp->main_mem.ready_cycles          = 50;
    configp->main_mem.send_chunk_cycles     = 15;
    configp->main_mem.chunk_size_bytes      = 8;
}

/* --- PRIVATE FUNCTION DEFINITIONS ----------------------------------------- */

/** @} addtogroup CONFIGDEFAULTS */

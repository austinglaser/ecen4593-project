/**
 * @file    MainMem.c
 * @author  Austin Glaser <austin@boulderes.com>
 * @brief   MainMem Source
 *
 * @addtogroup MAINMEM
 * @{
 */

/* --- PRIVATE DEPENDENCIES ------------------------------------------------- */

#include "MainMem.h"

#include "Config.h"

#include <stdbool.h>
#include <stdint.h>

/* --- PRIVATE CONSTANTS ---------------------------------------------------- */
/* --- PRIVATE DATATYPES ---------------------------------------------------- */
/* --- PRIVATE MACROS ------------------------------------------------------- */
/* --- PRIVATE FUNCTION PROTOTYPES ------------------------------------------ */
/* --- PUBLIC VARIABLES ----------------------------------------------------- */
/* --- PRIVATE VARIABLES ---------------------------------------------------- */

static config_t * config;

//static uint64_t unaligned_address_mask;

/* --- PUBLIC FUNCTIONS ----------------------------------------------------- */

void MainMem_Create(config_t * configp)
{
    config = configp;
}

uint32_t MainMem_Access(access_t * accessp)
{
    (void) accessp;
    //uint32_t access_cycles = config->main_mem.send_address_cycles +
    //                         config->main_mem.ready_cycles;

    //uint64_t address = accessp->address;

    return 75;
}

/* --- PRIVATE FUNCTION DEFINITIONS ----------------------------------------- */

/** @} addtogroup MAINMEM */

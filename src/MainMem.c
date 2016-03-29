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
#include "Util.h"

#include <inttypes.h>
#include <stdbool.h>
#include <stdint.h>

/* --- PRIVATE CONSTANTS ---------------------------------------------------- */
/* --- PRIVATE DATATYPES ---------------------------------------------------- */
/* --- PRIVATE MACROS ------------------------------------------------------- */
/* --- PRIVATE FUNCTION PROTOTYPES ------------------------------------------ */
/* --- PUBLIC VARIABLES ----------------------------------------------------- */
/* --- PRIVATE VARIABLES ---------------------------------------------------- */
/* --- PUBLIC FUNCTIONS ----------------------------------------------------- */

void MainMem_Create(main_mem_t * memp, config_t * configp)
{
    memp->configp               = &configp->main_mem;
}

uint32_t MainMem_Access(main_mem_t * memp, access_t * accessp)
{
    uint32_t access_cycles = memp->configp->send_address_cycles +
                             memp->configp->ready_cycles;

    access_t chunk_aligned_access;
    Access_Align(&chunk_aligned_access, accessp, memp->configp->chunk_size_bytes);
    uint32_t n_chunks = chunk_aligned_access.n_bytes / memp->configp->chunk_size_bytes;

    access_cycles += n_chunks * memp->configp->send_chunk_cycles;

    return access_cycles;
}

/* --- PRIVATE FUNCTION DEFINITIONS ----------------------------------------- */

/** @} addtogroup MAINMEM */

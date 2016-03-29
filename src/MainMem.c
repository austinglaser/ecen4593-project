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
    memp->chunk_alignment_mask  = BlockAlignmentMask(configp->main_mem.chunk_size_bytes);
}

uint32_t MainMem_Access(main_mem_t * memp, access_t * accessp)
{
    uint32_t access_cycles = memp->configp->send_address_cycles +
                             memp->configp->ready_cycles;

    uint64_t start_chunk = accessp->address & memp->chunk_alignment_mask;
    uint64_t end_chunk = ( ((accessp->address + accessp->n_bytes) - 1) &
                           memp->chunk_alignment_mask ) +
                         memp->configp->chunk_size_bytes;
    uint32_t aligned_n_bytes = end_chunk - start_chunk;
    uint32_t n_chunks = aligned_n_bytes / memp->configp->chunk_size_bytes;

    access_cycles += n_chunks * memp->configp->send_chunk_cycles;

    return access_cycles;
}

/* --- PRIVATE FUNCTION DEFINITIONS ----------------------------------------- */

/** @} addtogroup MAINMEM */

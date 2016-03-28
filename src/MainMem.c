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
    memp->mem_config = &configp->main_mem;
}

uint32_t MainMem_Access(main_mem_t * memp, access_t * accessp)
{
    uint32_t access_cycles = memp->mem_config->send_address_cycles +
                             memp->mem_config->ready_cycles;


    uint64_t address_alignment = accessp->address % memp->mem_config->chunk_size_bytes;
    uint32_t aligned_n_bytes = accessp->n_bytes + address_alignment;
    uint32_t n_chunks = CEIL_DIVIDE(aligned_n_bytes, memp->mem_config->chunk_size_bytes);

    access_cycles += n_chunks * memp->mem_config->send_chunk_cycles;

    return access_cycles;
}

/* --- PRIVATE FUNCTION DEFINITIONS ----------------------------------------- */

/** @} addtogroup MAINMEM */

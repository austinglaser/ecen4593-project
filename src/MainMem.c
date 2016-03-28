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

static config_t * config;

/* --- PUBLIC FUNCTIONS ----------------------------------------------------- */

void MainMem_Create(config_t * configp)
{
    config = configp;
}

uint32_t MainMem_Access(access_t * accessp)
{
    uint32_t access_cycles = config->main_mem.send_address_cycles +
                             config->main_mem.ready_cycles;


    uint64_t address_alignment = accessp->address % config->main_mem.chunk_size_bytes;
    uint32_t aligned_n_bytes = accessp->n_bytes + address_alignment;
    uint32_t n_chunks = CEIL_DIVIDE(aligned_n_bytes, config->main_mem.chunk_size_bytes);

    access_cycles += n_chunks * config->main_mem.send_chunk_cycles;

    return access_cycles;
}

/* --- PRIVATE FUNCTION DEFINITIONS ----------------------------------------- */

/** @} addtogroup MAINMEM */

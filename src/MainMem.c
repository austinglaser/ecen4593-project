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
#include <stdlib.h>

/* --- PRIVATE CONSTANTS ---------------------------------------------------- */
/* --- PRIVATE DATATYPES ---------------------------------------------------- */

struct _main_mem_t {
    memory_param_t const * configp;
};

/* --- PRIVATE MACROS ------------------------------------------------------- */
/* --- PRIVATE FUNCTION PROTOTYPES ------------------------------------------ */
/* --- PUBLIC VARIABLES ----------------------------------------------------- */
/* --- PRIVATE VARIABLES ---------------------------------------------------- */
/* --- PUBLIC FUNCTIONS ----------------------------------------------------- */

main_mem_t MainMem_Create(config_t const * configp)
{
    main_mem_t mem = (main_mem_t) malloc(sizeof(*mem));
    if (mem == NULL) {
        return NULL;
    }

    mem->configp = &configp->main_mem;

    return mem;
}

void MainMem_Destroy(main_mem_t mem)
{
    if (mem) free(mem);
}

uint32_t MainMem_Access(main_mem_t mem, access_t const * accessp)
{
    uint32_t access_cycles = mem->configp->send_address_cycles +
                             mem->configp->ready_cycles;

    access_t chunk_aligned_access;
    Access_Align(&chunk_aligned_access, accessp, mem->configp->chunk_size_bytes);
    uint32_t n_chunks = chunk_aligned_access.n_bytes / mem->configp->chunk_size_bytes;

    access_cycles += n_chunks * mem->configp->send_chunk_cycles;

    return access_cycles;
}

/* --- PRIVATE FUNCTION DEFINITIONS ----------------------------------------- */

/** @} addtogroup MAINMEM */

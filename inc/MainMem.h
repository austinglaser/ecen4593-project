/**
 * @file    MainMem.h
 * @author  Austin Glaser <austin@boulderes.com>
 * @brief   MainMem Interface
 */

#ifndef MAINMEM_H
#define MAINMEM_H

/**@defgroup MAINMEM MainMem
 * @{ 
 *
 * @brief
 */

/* --- PUBLIC DEPENDENCIES -------------------------------------------------- */

#include "Access.h"
#include "Config.h"

#include <stdbool.h>
#include <stdint.h>

/* --- PUBLIC CONSTANTS ----------------------------------------------------- */
/* --- PUBLIC DATATYPES ----------------------------------------------------- */

typedef struct {
    memory_param_t * configp;
    uint64_t chunk_alignment_mask;
} main_mem_t;

/* --- PUBLIC MACROS -------------------------------------------------------- */
/* --- PUBLIC VARIABLES ----------------------------------------------------- */
/* --- PUBLIC FUNCTIONS ----------------------------------------------------- */

void MainMem_Create(main_mem_t * memp, config_t * configp);
uint32_t MainMem_Access(main_mem_t * memp, access_t * accessp);

/** @} defgroup MAINMEM */

#endif /* ifndef MAINMEM_H */

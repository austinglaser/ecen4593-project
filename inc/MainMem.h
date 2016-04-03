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

typedef struct _main_mem_t * main_mem_t;

/* --- PUBLIC MACROS -------------------------------------------------------- */
/* --- PUBLIC VARIABLES ----------------------------------------------------- */
/* --- PUBLIC FUNCTIONS ----------------------------------------------------- */

main_mem_t MainMem_Create(config_t const * config);
void MainMem_Destroy(main_mem_t mem);
uint32_t MainMem_Access(main_mem_t mem, access_t const * access);

/** @} defgroup MAINMEM */

#endif /* ifndef MAINMEM_H */

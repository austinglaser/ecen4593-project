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
/* --- PUBLIC MACROS -------------------------------------------------------- */
/* --- PUBLIC VARIABLES ----------------------------------------------------- */
/* --- PUBLIC FUNCTIONS ----------------------------------------------------- */

void MainMem_Create(config_t * configp);
uint32_t MainMem_Access(access_t * accessp);

/** @} defgroup MAINMEM */

#endif /* ifndef MAINMEM_H */

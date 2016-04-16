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
 * @brief Simulates accesses to an external main memory
 */

/* --- PUBLIC DEPENDENCIES -------------------------------------------------- */

#include "Access.h"
#include "Config.h"

#include <stdbool.h>
#include <stdint.h>

/* --- PUBLIC CONSTANTS ----------------------------------------------------- */
/* --- PUBLIC DATATYPES ----------------------------------------------------- */

/**@brief   A main memory instance */
typedef struct _main_mem_t * main_mem_t;

/* --- PUBLIC MACROS -------------------------------------------------------- */
/* --- PUBLIC VARIABLES ----------------------------------------------------- */
/* --- PUBLIC FUNCTIONS ----------------------------------------------------- */

/**@brief   Creates a new main memory instance
 *
 * @param[in] config:   The memory's configuration parameters
 *
 * @return  The new memory instance, or NULL if memory allocation failed
 */
main_mem_t MainMem_Create(memory_param_t const * config);

/**@brief   Destroy an instance of main memory
 *
 * @param[in] mem:      The memory to free
 */
void MainMem_Destroy(main_mem_t mem);

/**@brief   Simulate an access to main memory
 *
 * @param[in] mem:      The memory instance to access
 * @param[in] access:   Access descriptor
 *
 * @return:             The time it took to retrieve and send the requested
 *                      data
 */
uint32_t MainMem_Access(main_mem_t mem, access_t const * access);

/** @} defgroup MAINMEM */

#endif /* ifndef MAINMEM_H */

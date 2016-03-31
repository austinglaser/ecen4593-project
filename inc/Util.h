/**
 * @file    Util.h
 * @author  Austin Glaser <austin@boulderes.com>
 * @brief   Util Interface
 */

#ifndef UTIL_H
#define UTIL_H

/**@defgroup UTIL Util
 * @{ 
 *
 * @brief
 */

/* --- PUBLIC DEPENDENCIES -------------------------------------------------- */

#include <stdbool.h>
#include <stdint.h>

/* --- PUBLIC CONSTANTS ----------------------------------------------------- */
/* --- PUBLIC DATATYPES ----------------------------------------------------- */
/* --- PUBLIC MACROS -------------------------------------------------------- */

#define UNUSED_VARIABLE(var)    ((void) (var))
#define ARRAY_ELEMENTS(a)       (sizeof(a)/sizeof((a)[0]))
#define IS_POWER_OF_TWO(v)      ( ((v) != 0) && ((v) & ((v) - 1)) == 0 )
#define CEIL_DIVIDE(x, y)       ( (x) == 0 ? 0 : ( 1 + ( ( (x) - 1 ) / (y) ) ) )

/* --- PUBLIC VARIABLES ----------------------------------------------------- */
/* --- PUBLIC FUNCTIONS ----------------------------------------------------- */

uint32_t HighestBitSet_Uint64(uint64_t value);

uint64_t BlockAlignmentMask(uint32_t block_size);

/** @} defgroup UTIL */

#endif /* ifndef UTIL_H */

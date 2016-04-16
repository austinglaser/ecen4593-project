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
 * @brief   Miscellaneous useful functions
 */

/* --- PUBLIC DEPENDENCIES -------------------------------------------------- */

#include <stdbool.h>
#include <stdint.h>

/* --- PUBLIC CONSTANTS ----------------------------------------------------- */
/* --- PUBLIC DATATYPES ----------------------------------------------------- */
/* --- PUBLIC MACROS -------------------------------------------------------- */

/**@brief   Designate @p var as unused, and silence GCC's warning */
#define UNUSED_VARIABLE(var)    ((void) (var))

/**@brief   Retrieve the size of array @p a allocated in the local scope
 *
 * @warning This should NOT be used on an array passed by reference
 */
#define ARRAY_ELEMENTS(a)       (sizeof(a)/sizeof((a)[0]))

/**@brief   Determines whether @p v is a power of two by twiddlin' some bits */
#define IS_POWER_OF_TWO(v)      ( ((v) != 0) && ((v) & ((v) - 1)) == 0 )

/**@brief   Performs an integer divide, rounding up if there is a remainder */
#define CEIL_DIVIDE(x, y)       ( (x) == 0 ? 0 : ( 1 + ( ( (x) - 1 ) / (y) ) ) )

/* --- PUBLIC VARIABLES ----------------------------------------------------- */
/* --- PUBLIC FUNCTIONS ----------------------------------------------------- */

/**@brief   Determines the highest bit set in @p value
 *
 * @note    The bit is zero-indexed from the lowest-order bit
 * @note    @p value is a 64-bit type, but this function works on all types of
 *          equal or smaller length that can be cast to an unsigned type (they
 *          won't be sign-extended)
 */
uint32_t HighestBitSet(uint64_t value);

/**@brief   Computes a mask suitable for aligning an address to @p block_size
 *
 * @ntoe    block_size must be a power of two
 */
uint64_t AlignmentMask(uint32_t block_size);

/** @} defgroup UTIL */

#endif /* ifndef UTIL_H */

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
#define IS_POWER_OF_TWO(v)      (((v) != 0) && ((v) & ((v) - 1)) == 0 )

/* --- PUBLIC VARIABLES ----------------------------------------------------- */
/* --- PUBLIC FUNCTIONS ----------------------------------------------------- */

/** @} defgroup UTIL */

#endif /* ifndef UTIL_H */

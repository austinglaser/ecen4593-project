/**
 * @file    Util.c
 * @author  Austin Glaser <austin@boulderes.com>
 * @brief   Util Source
 *
 * @addtogroup UTIL
 * @{
 */

/* --- PRIVATE DEPENDENCIES ------------------------------------------------- */

#include "Util.h"

#include "CException.h"
#include "CExceptionConfig.h"
#include "ExceptionTypes.h"

#include <stdbool.h>
#include <stdint.h>

/* --- PRIVATE CONSTANTS ---------------------------------------------------- */
/* --- PRIVATE DATATYPES ---------------------------------------------------- */
/* --- PRIVATE MACROS ------------------------------------------------------- */
/* --- PRIVATE FUNCTION PROTOTYPES ------------------------------------------ */
/* --- PUBLIC VARIABLES ----------------------------------------------------- */
/* --- PRIVATE VARIABLES ---------------------------------------------------- */
/* --- PUBLIC FUNCTIONS ----------------------------------------------------- */

uint32_t HighestBitSet_Uint64(uint64_t value)
{
    return 63 - __builtin_clzll(value);
}

uint32_t HighestBitSet_Uint32(uint32_t value)
{
    return 31 - __builtin_clz(value);
}

uint64_t AlignmentMask(uint32_t block_size)
{
    if (!IS_POWER_OF_TWO(block_size)) {
        ThrowHere(ARGUMENT_ERROR);
    }

    return ~(((uint64_t) block_size) - 1);
}

/* --- PRIVATE FUNCTION DEFINITIONS ----------------------------------------- */

/** @} addtogroup UTIL */

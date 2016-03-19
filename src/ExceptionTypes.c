/**
 * @file    ExceptionTypes.c
 * @author  Austin Glaser <austin@boulderes.com>
 * @brief   ExceptionTypes Source
 *
 * @addtogroup EXCEPTIONTYPES
 * @{
 */

/* --- PRIVATE DEPENDENCIES ------------------------------------------------- */

#include "ExceptionTypes.h"

#include <stdbool.h>
#include <stdint.h>

/* --- PRIVATE CONSTANTS ---------------------------------------------------- */
/* --- PRIVATE DATATYPES ---------------------------------------------------- */
/* --- PRIVATE MACROS ------------------------------------------------------- */
/* --- PRIVATE FUNCTION PROTOTYPES ------------------------------------------ */
/* --- PUBLIC VARIABLES ----------------------------------------------------- */

char const * const exception_descriptors[MAX_EXCEPTION_N] = {
    [NO_EXCEPTION]      = "No exception",
    [ARGUMENT_ERROR]    = "Bad function argument",
    [BAD_CONFIG_PARAM]  = "Bad parameter in configuration",
    [BAD_CONFIG_FILE]   = "Unable to open configuration file",
};

/* --- PRIVATE VARIABLES ---------------------------------------------------- */
/* --- PUBLIC FUNCTIONS ----------------------------------------------------- */
/* --- PRIVATE FUNCTION DEFINITIONS ----------------------------------------- */

/** @} addtogroup EXCEPTIONTYPES */

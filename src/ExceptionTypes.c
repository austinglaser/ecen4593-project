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
    [SYNTAX_ERROR]      = "Invalid syntax",
    [BAD_CONFIG_CACHE]  = "Bad cache specifier in configuration",
    [BAD_CONFIG_PARAM]  = "Bad parameter name in configuration",
    [BAD_CONFIG_VALUE]  = "Invalid value for field in configuration",
    [BAD_CONFIG_FILE]   = "Unable to open configuration file",
};

/* --- PRIVATE VARIABLES ---------------------------------------------------- */
/* --- PUBLIC FUNCTIONS ----------------------------------------------------- */
/* --- PRIVATE FUNCTION DEFINITIONS ----------------------------------------- */

/** @} addtogroup EXCEPTIONTYPES */
